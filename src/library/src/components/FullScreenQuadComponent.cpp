#include "StdAfx.h"
#include "library/components/FullScreenQuadComponent.h"

#include "library/effect/Effect.h"
#include "library/effect/EffectMaterial.h"
#include "library/effect/EffectTechnique.h"
#include "library/effect/EffectPass.h"

#include "library/VertexTypes.h"
#include "library/Application.h"
#include "library/Exception.h"

#include <array>
#include <cassert>

namespace library
{
	FullScreenQuadComponent::FullScreenQuadComponent(const Application& app)
		: DrawableComponent(app)
	{
	}

	FullScreenQuadComponent::~FullScreenQuadComponent() = default;

	void FullScreenQuadComponent::SetEffectMaterial(
		EffectMaterial& material,
		const std::string& techniqueName,
		const std::string& passName
	)
	{
		if (m_material != &material)
		{
			m_material = &material;
			SetActiveTechnique(techniqueName, passName);
		}
	}

	void FullScreenQuadComponent::SetActiveTechnique(const std::string& techniqueName, const std::string& passName)
	{
		assert(!!m_material);

		auto& technique = m_material->GetEffect().GetTechnique(techniqueName);
		auto& pass = technique.GetPass(passName);

		m_pass = &pass;
		m_inputLayout = m_material->GetInputLayout(pass);
	}

	void FullScreenQuadComponent::SetMaterialUpdateFunction(const MaterialUpdateFunction& func)
	{
		m_materialUpdateFunction = func;
	}

	void FullScreenQuadComponent::Initialize()
	{
		assert(!!m_material && !!m_pass);

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
			m_verticesCount = vertices.size();

			D3D11_BUFFER_DESC vertexBufferDesc{};
			vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_verticesCount;
			vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
			vertexSubResourceData.pSysMem = vertices.data();

			auto hr = m_app.GetDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexSubResourceData,
				m_vertexBuffer.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateBuffer", hr);
			}
		}

		// index buffer
		{
			std::array<unsigned, 6> indices =
			{
				0, 1, 2,
				0, 2, 3,
			};
			m_indicesCount = indices.size();

			D3D11_BUFFER_DESC indexBufferDesc{};
			indexBufferDesc.ByteWidth = sizeof(unsigned) * m_indicesCount;
			indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA indexSubResourceData{};
			indexSubResourceData.pSysMem = indices.data();

			auto hr = m_app.GetDevice()->CreateBuffer(
				&indexBufferDesc,
				&indexSubResourceData,
				m_indexBuffer.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateBuffer", hr);
			}
		}
	}

	void FullScreenQuadComponent::SetEffectData()
	{
		if (!!m_materialUpdateFunction)
		{
			m_materialUpdateFunction();
		}

		m_pass->Apply(0, m_app.GetDeviceContext().Get());
	}
} // namespace library
