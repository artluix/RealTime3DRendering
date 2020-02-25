#include "Materials/SkinnedModelMaterial.h"

#include <library/Effect/Effect.h>
#include <library/Model/Mesh.h>

using namespace library;

SkinnedModelMaterial::SkinnedModelMaterial(Effect& effect)
	: LightMaterial(effect)

	, m_boneTransforms(effect.GetVariable("BoneTransforms"))
{}

SkinnedModelMaterial::~SkinnedModelMaterial() = default;

//-------------------------------------------------------------------------

void SkinnedModelMaterial::InitializeInternal()
{
	EffectPass::InputElementDescArray inputElementDescriptions =
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

	CreateInputLayout(inputElementDescriptions, GetDefaultTechniqueName());
}

VertexBufferData SkinnedModelMaterial::CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh)
	const
{
	if (!mesh.HasVertices())
		return VertexBufferData();

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

		math::Vector4 weights;
		math::Vector4u indices;

		for (unsigned j = 0, count = vertexWeights.GetCount(); j < count; j++)
		{
			weights[j] = vertexWeights[j].weight;
			indices[j] = vertexWeights[j].boneIndex;
		}

		vertices.emplace_back(
			math::Vector4(position, 1.0f),
			uv.xy,
			normal,
			indices,
			weights
		);
	}

	return VertexBufferData(device, vertices);
}
