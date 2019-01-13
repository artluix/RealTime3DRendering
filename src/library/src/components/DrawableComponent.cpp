#include "StdAfx.h"
#include "library/components/DrawableComponent.h"

#include "library/effect/EffectMaterial.h"
#include "library/effect/EffectPass.h"
#include "library/effect/EffectTechnique.h"

#include "library/Model.h"
#include "library/Mesh.h"

#include "library/Application.h"
#include "library/Utils.h"
#include "library/Exception.h"

#include <DDSTextureLoader.h>
#include <cassert>

namespace library
{
	DrawableComponent::DrawableComponent(const Application& app)
		: Component(app)
	{
	}

	void DrawableComponent::Initialize()
	{
		if (const auto material = GetEffectMaterial())
		{
			assert(material->IsInitialized());

			// model
			if (m_modelPath)
			{
				Model model(m_app, m_modelPath, true);

				const auto& mesh = model.GetMesh(0);
				m_vertexBuffer = material->CreateVertexBuffer(m_app.GetD3DDevice(), mesh);
				m_indexBuffer = mesh.CreateIndexBuffer();
				m_indicesCount = mesh.GetIndicesCount();
			}
		}

		// texture
		if (m_texturePath)
		{
			std::vector<library::byte> textureData;
			utils::LoadBinaryFile(m_texturePath, textureData);
			if (textureData.empty())
			{
				throw Exception("Load texture failed.");
			}

			auto hr = DirectX::CreateDDSTextureFromMemory(
				m_app.GetD3DDevice(),
				reinterpret_cast<const std::uint8_t*>(textureData.data()),
				textureData.size(),
				nullptr,
				m_textureShaderResourceView.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("CreateDDSTextureFromMemory() failed.", hr);
			}
		}

		m_app.GetRenderer()->RegisterForRender(this);
	}

	void DrawableComponent::Draw(const Time& time)
	{
		SetIA();
		SetEffectData();
		Render();
	}

	void DrawableComponent::SetIA()
	{
		if (const auto material = GetEffectMaterial())
		{
			auto deviceContext = m_app.GetD3DDeviceContext();
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			const auto& currentTechnique = material->GetCurrentTechnique();
			const auto& pass = currentTechnique.GetPass(0);

			auto inputLayout = material->GetInputLayout(pass);
			deviceContext->IASetInputLayout(inputLayout);

			unsigned stride = material->GetVertexSize();
			unsigned offset = 0;
			deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
			deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
		}
	}

	void DrawableComponent::SetEffectData()
	{
		if (auto material = GetEffectMaterial())
		{
			auto& currentTechnique = material->GetCurrentTechnique();
			auto& pass = currentTechnique.GetPass(0);

			pass.Apply(0, m_app.GetD3DDeviceContext());
		}
	}

	void DrawableComponent::Render()
	{
		m_app.GetD3DDeviceContext()->DrawIndexed(m_indicesCount, 0, 0);
	}

	void DrawableComponent::SetVisible(const bool visible)
	{
		m_visible = visible;
	}

	void DrawableComponent::SetCamera(const CameraComponent& camera)
	{
		if (m_camera != &camera)
			m_camera = &camera;
	}

	void DrawableComponent::SetModelPath(const Path& path)
	{
		if (m_modelPath != path)
			m_modelPath = path;
	}

	void DrawableComponent::SetTexturePath(const Path& path)
	{
		if (m_texturePath != path)
			m_texturePath = path;
	}
} // namespace library
