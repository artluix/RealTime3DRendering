#include "StdAfx.h"
#include "library/components/DrawableComponent.h"

#include "library/Application.h"
#include "library/Utils.h"
#include "library/Exception.h"

#include <cassert>

namespace library
{
	DrawableComponent::DrawableComponent(const Application& app)
		: Component(app)
	{
	}

	void DrawableComponent::Initialize()
	{
		LoadModel(m_modelPath, m_vertexBuffer, m_indexBuffer);
		LoadTexture(m_texturePath, m_textureShaderResourceView);

		// set default input layout
		if (auto material = GetMaterial())
		{
			const auto& currentTechnique = material->GetCurrentTechnique();
			const auto& pass = currentTechnique.GetPass(0);

			m_inputLayout = material->GetInputLayoutShared(pass);
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
		if (const auto material = GetMaterial())
		{
			auto deviceContext = m_app.GetDeviceContext();
			deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			deviceContext->IASetInputLayout(m_inputLayout.Get());

			if (!!m_vertexBuffer)
			{
				unsigned stride = material->GetVertexSize();
				unsigned offset = 0;
				deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
			}

			if (!!m_indexBuffer)
			{
				deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
			}
		}
	}

	void DrawableComponent::SetEffectData()
	{
		if (auto material = GetMaterial())
		{
			auto& currentTechnique = material->GetCurrentTechnique();
			auto& pass = currentTechnique.GetPass(0);

			pass.Apply(0, m_app.GetDeviceContext());
		}
	}

	void DrawableComponent::Render()
	{
		auto deviceContext = m_app.GetDeviceContext();

		if (m_indicesCount > 0)
			deviceContext->DrawIndexed(m_indicesCount, 0, 0);
		else
			deviceContext->Draw(m_verticesCount, 0);
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
} // namespace library
