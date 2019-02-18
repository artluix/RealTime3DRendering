#include "StdAfx.h"
#include "library/components/FullScreenQuadComponent.h"

#include "library/effect/Effect.h"
#include "library/effect/EffectTechnique.h"
#include "library/effect/EffectPass.h"

#include "library/materials/Material.h"

#include "library/VertexTypes.h"
#include "library/Application.h"
#include "library/Exception.h"

#include <array>
#include <cassert>

namespace library
{
	FullScreenQuadComponent::FullScreenQuadComponent()
	{
		SetAutoRenderable(false);
	}

	FullScreenQuadComponent::~FullScreenQuadComponent() = default;

	//-------------------------------------------------------------------------

	void FullScreenQuadComponent::SetMaterial(Material& material)
	{
		if (m_material != &material)
		{
			m_material = &material;
		}
	}

	void FullScreenQuadComponent::SetMaterial(Material& material, const std::string& techniqueName, const std::string& passName /* = "p0"*/)
	{
		SetMaterial(material);
		SetActiveTechnique(techniqueName, passName);
	}

	//-------------------------------------------------------------------------

	void FullScreenQuadComponent::SetActiveTechnique(const std::string& techniqueName, const std::string& passName /* = "p0"*/)
	{
		assert(!!m_material);

		auto& technique = m_material->GetEffect().GetTechnique(techniqueName);
		auto& pass = technique.GetPass(passName);

		m_pass = &pass;
		m_inputLayout = m_material->GetInputLayoutShared(pass);
	}

	void FullScreenQuadComponent::SetMaterialUpdateFunction(const MaterialUpdateFunction& func)
	{
		m_materialUpdateFunction = func;
	}

	void FullScreenQuadComponent::Initialize(const Application& app)
	{
		DrawableMaterialComponent::Initialize(app);

		// vertex buffer
		{
			using Vertex = VertexPositionTexture;

			std::array<Vertex, 4> vertices =
			{
				Vertex(DirectX::XMFLOAT4(-1.0f, -1.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 1.0f)),
				Vertex(DirectX::XMFLOAT4(-1.0f, 1.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(0.0f, 0.0f)),
				Vertex(DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 0.0f)),
				Vertex(DirectX::XMFLOAT4(1.0f, -1.0f, 0.0f, 1.0f), DirectX::XMFLOAT2(1.0f, 1.0f)),
			};
			m_vertexBufferData.count = vertices.size();

			D3D11_BUFFER_DESC vertexBufferDesc{};
			vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertexBufferData.count;
			vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
			vertexSubResourceData.pSysMem = vertices.data();

			auto hr = app.GetDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexSubResourceData,
				m_vertexBufferData.buffer.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateBuffer", hr);
			}
		}

		// index buffer
		{
			m_indexBufferData = std::make_unique<BufferData>();

			std::array<unsigned, 6> indices =
			{
				0, 1, 2,
				0, 2, 3,
			};
			m_indexBufferData->count = indices.size();

			D3D11_BUFFER_DESC indexBufferDesc{};
			indexBufferDesc.ByteWidth = sizeof(unsigned) * m_indexBufferData->count;
			indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA indexSubResourceData{};
			indexSubResourceData.pSysMem = indices.data();

			auto hr = app.GetDevice()->CreateBuffer(
				&indexBufferDesc,
				&indexSubResourceData,
				m_indexBufferData->buffer.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateBuffer", hr);
			}
		}
	}

	void FullScreenQuadComponent::Draw_SetData()
	{
		if (!!m_materialUpdateFunction)
		{
			m_materialUpdateFunction();
		}

		m_pass->Apply(0, m_app->GetDeviceContext());
	}
} // namespace library
