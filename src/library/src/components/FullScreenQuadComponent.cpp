#include "StdAfx.h"
#include "library/Components/FullScreenQuadComponent.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectTechnique.h"
#include "library/Effect/EffectPass.h"

#include "library/Materials/Material.h"

#include "library/VertexTypes.h"
#include "library/Application.h"
#include "library/Exception.h"

#include <array>
#include <cassert>

namespace library
{
	FullScreenQuadComponent::FullScreenQuadComponent() = default;
	FullScreenQuadComponent::~FullScreenQuadComponent() = default;

	//-------------------------------------------------------------------------

	void FullScreenQuadComponent::SetMaterial(Material& material)
	{
		m_material = &material;
	}

	void FullScreenQuadComponent::SetMaterial(
		Material& material,
		const std::string& techniqueName,
		const std::string& passName /* = "p0"*/
	)
	{
		SetMaterial(material);
		SetActiveTechnique(techniqueName, passName);
	}

	//-------------------------------------------------------------------------

	void FullScreenQuadComponent::SetActiveTechnique(
		const std::string& techniqueName,
		const std::string& passName /* = "p0"*/
	)
	{
		assert(!!m_material);

		const auto& technique = m_material->GetEffect().GetTechnique(techniqueName);
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
		SceneComponent::Initialize(app);

		m_meshesData = { MeshData() };
		auto& md = m_meshesData.front();

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
			md.vertexBuffer.elementsCount = vertices.size();

			D3D11_BUFFER_DESC vertexBufferDesc{};
			vertexBufferDesc.ByteWidth = sizeof(Vertex) * md.vertexBuffer.elementsCount;
			vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

			D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
			vertexSubResourceData.pSysMem = vertices.data();

			auto hr = app.GetDevice()->CreateBuffer(
				&vertexBufferDesc,
				&vertexSubResourceData,
				md.vertexBuffer.buffer.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateBuffer", hr);
			}
		}

		// index buffer
		{
			md.indexBuffer = std::make_optional(BufferData());

			std::array<unsigned, 6> indices =
			{
				0, 1, 2,
				0, 2, 3,
			};
			md.indexBuffer->elementsCount = indices.size();

			D3D11_BUFFER_DESC indexBufferDesc{};
			indexBufferDesc.ByteWidth = sizeof(unsigned) * md.indexBuffer->elementsCount;
			indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA indexSubResourceData{};
			indexSubResourceData.pSysMem = indices.data();

			auto hr = app.GetDevice()->CreateBuffer(
				&indexBufferDesc,
				&indexSubResourceData,
				md.indexBuffer->buffer.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateBuffer", hr);
			}
		}
	}

	void FullScreenQuadComponent::Update(const Time& time)
	{
	}

	void FullScreenQuadComponent::Draw_SetData(const MeshData& meshData)
	{
		if (!!m_materialUpdateFunction)
			m_materialUpdateFunction();

		m_pass->Apply(0, GetApp()->GetDeviceContext());
	}

	unsigned FullScreenQuadComponent::GetVertexSize() const
	{
		return sizeof(VertexPositionTexture);
	}
} // namespace library
