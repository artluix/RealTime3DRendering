#include "StdAfx.h"
#include "library/Materials/NormalMappingMaterial.h"

#include "library/Effect/Effect.h"
#include "library/Model/Mesh.h"

namespace library
{
NormalMappingMaterial::NormalMappingMaterial(Effect& effect)
	: DirectionalLightMaterial(effect, "main10")

	, m_normalMap(effect.GetVariable("NormalMap"))
{}

NormalMappingMaterial::~NormalMappingMaterial() = default;

//-------------------------------------------------------------------------

void NormalMappingMaterial::InitializeInternal()
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
			"TANGENT",
			0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0
		},
	};

	Material::InitializeInternal();
}

VertexBufferData NormalMappingMaterial::CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh)
	const
{
	if (!mesh.HasVertices())
		return VertexBufferData();

	assert(mesh.HasTangentBinormals());

	std::vector<Vertex> vertices;

	const auto& meshVertices = mesh.GetVertices();
	const auto& textureCoordinates = mesh.GetTextureCoordinates(0);
	const auto& normals = mesh.GetNormals();
	const auto& tangentBinormals = mesh.GetTangentBinormals();
	const auto verticesCount = meshVertices.size();

	vertices.reserve(verticesCount);

	for (unsigned i = 0; i < verticesCount; i++)
	{
		const auto& position = meshVertices[i];
		const auto& uv = textureCoordinates[i];
		const auto& normal = normals[i];
		const auto& tangent = tangentBinormals[i].tangent;

		vertices.emplace_back(
			math::Vector4(position, 1.0f),
			uv.xy,
			normal,
			tangent
		);
	}

	return VertexBufferData(device, vertices);
}
} // namespace library
