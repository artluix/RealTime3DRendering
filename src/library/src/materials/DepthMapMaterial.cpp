#include "StdAfx.h"
#include "library/Materials/DepthMapMaterial.h"

#include "library/Effect/Effect.h"

#include "library/Model/Model.h"
#include "library/Model/Mesh.h"

namespace library
{
DepthMapMaterial::DepthMapMaterial(Effect& effect)
	: Material(effect, "create_depth_map")
	, m_worldLightViewProjection(effect.GetVariable("worldLightViewProjection"))
{}

DepthMapMaterial::~DepthMapMaterial() = default;

//-------------------------------------------------------------------------

void DepthMapMaterial::InitializeInternal()
{
	m_inputElementDescriptions = {
		{
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
		}
	};

	Material::InitializeInternal();

	// CreateInputLayout("depth_map_render_target");
	CreateInputLayout("depth_map_bias");
}

VertexBufferData DepthMapMaterial::CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const
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
