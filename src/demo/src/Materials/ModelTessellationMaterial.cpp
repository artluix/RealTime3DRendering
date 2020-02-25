#include "Materials/ModelTessellationMaterial.h"

#include <library/Effect/Effect.h>
#include "library/Effect/EffectTechnique.h"

#include <library/Model/Mesh.h>

using namespace library;

ModelTessellationMaterial::ModelTessellationMaterial(Effect& effect)
	: TessellationMaterial(effect, "solid_color_manual_factors")

	, m_world(effect.GetVariable("World"))
	, m_cameraPosition(effect.GetVariable("CameraPosition"))
	, m_colorTexture(effect.GetVariable("ColorTexture"))
	, m_maxTessFactor(effect.GetVariable("MaxTessFactor"))
	, m_minTessDistance(effect.GetVariable("MinTessDistance"))
	, m_maxTessDistance(effect.GetVariable("MaxTessDistance"))
{
}

void ModelTessellationMaterial::InitializeInternal()
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
		}
	};

	for (unsigned i = 0, techniquesCount = GetEffect().GetTechniquesCount(); i < techniquesCount; i++)
	{
		auto& technique = GetEffect().GetTechnique(i);

		for (unsigned j = 0, passesCount = technique.GetPassesCount(); j < passesCount; j++)
		{
			auto& pass = technique.GetPass(j);
			CreateInputLayout(inputElementDescriptions, pass);
		}
	}
}

VertexBufferData ModelTessellationMaterial::CreateVertexBufferData(
	ID3D11Device* const device,
	const Mesh& mesh) const
{
	if (!mesh.HasVertices())
		return VertexBufferData();

	std::vector<Vertex> vertices;

	const auto& meshVertices = mesh.GetVertices();
	const auto& textureCoordinates = mesh.GetTextureCoordinates(0);
	const auto verticesCount = meshVertices.size();

	vertices.reserve(verticesCount);

	for (unsigned i = 0; i < verticesCount; i++)
	{
		const auto& position = meshVertices[i];
		const auto& uv = textureCoordinates[i];

		vertices.emplace_back(
			math::Vector4(position, 1.0f),
			uv.xy
		);
	}

	return VertexBufferData(device, vertices);
}
