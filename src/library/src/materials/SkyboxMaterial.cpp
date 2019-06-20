#include "StdAfx.h"
#include "library/Materials/SkyboxMaterial.h"

#include "library/Effect/Effect.h"
#include "library/Model/Mesh.h"
#include "library/Math/Color.h"

namespace library
{
SkyboxMaterial::SkyboxMaterial(Effect& effect)
	: Material(effect, "main11")
	, m_worldViewProjection(effect.GetVariable("WorldViewProjection"))
	, m_skyboxTexture(effect.GetVariable("SkyboxTexture"))
{}

SkyboxMaterial::~SkyboxMaterial() = default;

void SkyboxMaterial::InitializeInternal()
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
	};

	Material::InitializeInternal();
}

VertexBufferData SkyboxMaterial::CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const
{
	if (!mesh.HasVertices())
		return VertexBufferData();

	std::vector<Vertex> vertices;

	const auto& meshVertices = mesh.GetVertices();
	const auto verticesCount = meshVertices.size();

	vertices.reserve(verticesCount);

	for (unsigned i = 0; i < verticesCount; i++)
	{
		const auto& position = meshVertices[i];
		vertices.emplace_back(math::Vector4(position, 1.0f));
	}

	return VertexBufferData(device, vertices);
}
} // namespace library
