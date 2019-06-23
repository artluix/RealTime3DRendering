#include "Materials/DiffuseLightingMaterial.h"

#include <library/Effect/Effect.h>
#include <library/Model/Mesh.h>

using namespace library;

DiffuseLightingMaterial::DiffuseLightingMaterial(Effect& effect)
	: Material(effect, "main10")

	, m_ambientColor(effect.GetVariable("ambientColor"))
	, m_lightColor(effect.GetVariable("lightColor"))
	, m_lightDirection(effect.GetVariable("lightDirection"))

	, m_wvp(effect.GetVariable("wvp"))
	, m_world(effect.GetVariable("world"))

	, m_colorTexture(effect.GetVariable("ColorTexture"))
{}

DiffuseLightingMaterial::~DiffuseLightingMaterial() = default;

void DiffuseLightingMaterial::InitializeInternal()
{
	m_inputElementDescriptions = {
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
	};

	Material::InitializeInternal();
}

VertexBufferData DiffuseLightingMaterial::CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh)
	const
{
	if (!mesh.HasVertices())
		return VertexBufferData();

	std::vector<Vertex> vertices;

	const auto& meshVertices = mesh.GetVertices();
	const auto& textureCoordinates = mesh.GetTextureCoordinates(0);
	const auto& normals = mesh.GetNormals();
	const auto verticesCount = meshVertices.size();

	vertices.reserve(verticesCount);

	for (unsigned i = 0; i < verticesCount; i++)
	{
		const auto& position = meshVertices[i];
		const auto& uv = textureCoordinates[i];
		const auto& normal = normals[i];

		vertices.emplace_back(
			math::Vector4(position, 1.0f),
			uv.xy,
			normal
		);
	}

	return VertexBufferData(device, vertices);
}