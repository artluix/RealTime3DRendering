#include "StdAfx.h"
#include "library/effects/SkyboxMaterial.h"

#include "library/effects/Effect.h"
#include "library/Mesh.h"
#include "library/Color.h"
#include "library/Exception.h"

namespace library
{
	SkyboxMaterial::SkyboxMaterial(const Effect& effect)
		: Class(effect, "main11")
		, m_worldViewProjection(nullptr)
		, m_skyboxTexture(nullptr)
	{
	}

	void SkyboxMaterial::Initialize()
	{
		EffectMaterial::Initialize();

		m_worldViewProjection = &GetEffect().GetVariable("WorldViewProjection");
		m_skyboxTexture = &GetEffect().GetVariable("SkyboxTexture");

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptions =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		CreateInputLayout("main11", "p0", inputElementDescriptions);
	}

	ComPtr<ID3D11Buffer> SkyboxMaterial::CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const
	{
		if (!mesh.HasVertices())
			return ComPtr<ID3D11Buffer>();

		Vertices vertices;

		const auto& meshVertices = mesh.GetVertices();
		const auto verticesCount = meshVertices.size();

		vertices.reserve(verticesCount);

		for (unsigned i = 0; i < verticesCount; i++)
		{
			const auto& position = meshVertices[i];
			vertices.emplace_back(DirectX::XMFLOAT4(position.x, position.y, position.z, 1.0f));
		}

		return CreateVertexBuffer(device, vertices);
	}

	ComPtr<ID3D11Buffer> SkyboxMaterial::CreateVertexBuffer(ID3D11Device* const device, const Vertices& vertices) const
	{
		ComPtr<ID3D11Buffer> vertexBuffer;

		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData{};
		vertexSubResourceData.pSysMem = vertices.data();

		auto hr = device->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, vertexBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateBuffer() failed.");
		}

		return vertexBuffer;
	}

} // namespace library
