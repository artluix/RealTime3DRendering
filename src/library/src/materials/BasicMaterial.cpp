#include "StdAfx.h"
#include "library/materials/BasicMaterial.h"

#include "library/effect/Effect.h"
#include "library/Mesh.h"
#include "library/Color.h"

namespace library
{
	BasicMaterial::BasicMaterial(Effect& effect)
		: Material(effect, "main11")
		, m_worldViewProjection(effect.GetVariable("WorldViewProjection"))
	{
	}

	BasicMaterial::~BasicMaterial() = default;

	//-------------------------------------------------------------------------

	void BasicMaterial::InitializeInternal()
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptions =
		{
			{
				"POSITION",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				0,
				D3D11_INPUT_PER_VERTEX_DATA,
				0 
			},
			{
				"COLOR",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0 
			},
		};

		CreateInputLayout(inputElementDescriptions);
	}

	ComPtr<ID3D11Buffer> BasicMaterial::CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const
	{
		if (!mesh.HasVertices())
			return ComPtr<ID3D11Buffer>();

		std::vector<Vertex> vertices;

		const auto& meshVertices = mesh.GetVertices();
		const auto verticesCount = meshVertices.size();

		vertices.reserve(verticesCount);

		if (mesh.HasVerticesColors())
		{
			const auto& vertexColors = mesh.GetVertexColors(0);

			for (unsigned i = 0; i < verticesCount; i++)
			{
				const auto& position = meshVertices[i];
				const auto& color = vertexColors[i];
				vertices.emplace_back(DirectX::XMFLOAT4(position.x, position.y, position.z, 1.0f), color);
			}
		}
		else
		{
			for (unsigned i = 0; i < verticesCount; i++)
			{
				const auto& position = meshVertices[i];
				const auto& color = DirectX::XMFLOAT4(Color::White);
				vertices.emplace_back(DirectX::XMFLOAT4(position.x, position.y, position.z, 1.0f), color);
			}
		}

		return Material::CreateVertexBuffer(device, vertices.data(), vertices.size() * sizeof(Vertex));
	}
} // namespace library
