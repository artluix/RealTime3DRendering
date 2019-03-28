#include "StdAfx.h"
#include "library/Materials/SkinnedModelMaterial.h"

#include "library/Effect/Effect.h"
#include "library/Model/Mesh.h"

namespace library
{
	SkinnedModelMaterial::SkinnedModelMaterial(Effect& effect)
		: PointLightMaterial(effect)

		, m_boneTransforms(effect.GetVariable("boneTransforms"))
	{
	}

	SkinnedModelMaterial::~SkinnedModelMaterial() = default;

	//-------------------------------------------------------------------------

	void SkinnedModelMaterial::InitializeInternal()
	{
		m_inputElementDescriptions =
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
				"TEXCOORD",
				0,
				DXGI_FORMAT_R32G32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			{
				"NORMAL",
				0,
				DXGI_FORMAT_R32G32B32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			{
				"BONEINDICES",
				0,
				DXGI_FORMAT_R32G32B32A32_UINT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			},
			{
				"WEIGHTS",
				0,
				DXGI_FORMAT_R32G32B32A32_FLOAT,
				0,
				D3D11_APPEND_ALIGNED_ELEMENT,
				D3D11_INPUT_PER_VERTEX_DATA,
				0
			}
		};

		CreateInputLayout();
	}

	ComPtr<ID3D11Buffer> SkinnedModelMaterial::CreateVertexBuffer(
		ID3D11Device* const device,
		const Mesh& mesh
	) const
	{
		if (!mesh.HasVertices())
			return ComPtr<ID3D11Buffer>();

		std::vector<Vertex> vertices;

		const auto& meshVertices = mesh.GetVertices();
		const auto& textureCoordinates = mesh.GetTextureCoordinates(0);
		const auto& normals = mesh.GetNormals();
		const auto& boneWeights = mesh.GetBoneWeights();

		const auto verticesCount = meshVertices.size();
		vertices.reserve(verticesCount);

		for (unsigned i = 0; i < verticesCount; i++)
		{
			const auto& position = meshVertices[i];
			const auto& uv = textureCoordinates[i];
			const auto& normal = normals[i];
			const auto& vertexWeights = boneWeights[i];

			std::array<float, Bone::VertexWeights::MaxPerVertex> weights;
			std::array<unsigned, Bone::VertexWeights::MaxPerVertex> indices;

			weights.fill(0.f);

			for (unsigned j = 0, sz = vertexWeights.Size(); j < sz; j++)
			{
				weights[j] = vertexWeights[j].weight;
				indices[j] = vertexWeights[j].boneIndex;
			}

			vertices.emplace_back(
				DirectX::XMFLOAT4(position.x, position.y, position.z, 1.0f),
				DirectX::XMFLOAT2(uv.x, uv.y),
				normal,
				DirectX::XMUINT4(indices.data()),
				DirectX::XMFLOAT4(weights.data())
			);
		}

		return Material::CreateVertexBuffer(device, vertices);
	}
} // namespace library
