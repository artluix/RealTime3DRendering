#include "Materials/MultiplePointLightsMaterial.h"

#include <library/Effect/Effect.h>
#include <library/Model/Mesh.h>

using namespace library;

MultiplePointLightsMaterial::MultiplePointLightsMaterial(Effect& effect)
	: LightMaterial(effect, "forward")

	, m_lightsData(effect.GetVariable("lightsData"))
	, m_lightsCount(effect.GetVariable("lightsCount"))

	, m_colorBufferTexture(effect.GetVariable("ColorBufferTexture"))
	, m_normalBufferTexture(effect.GetVariable("NormalBufferTexture"))
	, m_worldPositionBufferTexture(effect.GetVariable("WorldPositionBufferTexture"))
	, m_depthBufferTexture(effect.GetVariable("DepthBufferTexture"))
{
}

void MultiplePointLightsMaterial::InitializeInternal()
{
	// vertex buffer input
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
		};

		CreateInputLayout(inputElementDescriptions, "forward");
		CreateInputLayout(inputElementDescriptions, "deferred", "geometry");
	}

	//-------------------------------------------------------------------------

	// vertex buffer for 2nd pass of deferred
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
		};

		CreateInputLayout(inputElementDescriptions, "deferred", "light");
	}
}

VertexBufferData MultiplePointLightsMaterial::CreateVertexBufferData(
	ID3D11Device* const device,
	const Mesh& mesh
) const
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

