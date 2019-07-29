#include "Materials/TessellationMaterial.h"

#include <library/Effect/Effect.h>
#include <library/Model/Mesh.h>

using namespace library;

TessellationMaterial::TessellationMaterial(
	Effect& effect,
	const std::string& defaultTechniqueName /* = "main11" */
)
	: Material(effect, defaultTechniqueName)

	, m_wvp(effect.GetVariable("wvp"))
	, m_tessellationEdgeFactors(effect.GetVariable("tessellationEdgeFactors"))
	, m_tessellationInsideFactors(effect.GetVariable("tessellationInsideFactor"))
{}

//-------------------------------------------------------------------------

void TessellationMaterial::InitializeInternal()
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
		}
	};

	CreateInputLayout(inputElementDescriptions, GetDefaultTechniqueName());
}

VertexBufferData TessellationMaterial::CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh)
	const
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
