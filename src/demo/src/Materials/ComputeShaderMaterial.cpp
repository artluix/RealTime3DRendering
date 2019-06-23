#include "Materials/ComputeShaderMaterial.h"

#include <library/Effect/Effect.h>

#include <library/Model/Mesh.h>

using namespace library;

ComputeShaderMaterial::ComputeShaderMaterial(Effect& effect)
	: Material(effect, "compute")
	, m_textureSize(effect.GetVariable("textureSize"))
	, m_blueColor(effect.GetVariable("blueColor"))
	, m_outputTexture(effect.GetVariable("OutputTexture"))

	, m_colorTexture(effect.GetVariable("ColorTexture"))
{
}

void ComputeShaderMaterial::InitializeInternal()
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
	};

	CreateInputLayout("render");
	//CreateInputLayout("compute");
}

VertexBufferData ComputeShaderMaterial::CreateVertexBufferData(
	ID3D11Device* const device,
	const Mesh& mesh
) const
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
