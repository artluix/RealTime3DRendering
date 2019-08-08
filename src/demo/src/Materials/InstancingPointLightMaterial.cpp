#include "Materials/InstancingPointLightMaterial.h"

#include <library/Effect/Effect.h>
#include <library/Model/Mesh.h>

using namespace library;

InstancingPointLightMaterial::InstancingPointLightMaterial(Effect& effect)
	: Material(effect, "main11")

	, m_ambientColor(effect.GetVariable("ambientColor"))

	, m_lightColor(effect.GetVariable("lightColor"))
	, m_lightPosition(effect.GetVariable("lightPosition"))
	, m_lightRadius(effect.GetVariable("lightRadius"))

	, m_cameraPosition(effect.GetVariable("cameraPosition"))
	, m_viewProjection(effect.GetVariable("viewProjection"))

	, m_colorTexture(effect.GetVariable("ColorTexture"))
{
}

InstancingPointLightMaterial::~InstancingPointLightMaterial() = default;

//-------------------------------------------------------------------------


void InstancingPointLightMaterial::InitializeInternal()
{
	EffectPass::InputElementDescArray inputElementDescriptions =
	{
		// vertex buffer data layout
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
		// instance data layout
		{
			"WORLD",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1,
			0,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		},
		{
			"WORLD",
			1,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1,
			16,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		},
		{
			"WORLD",
			2,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1,
			32,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		},
		{
			"WORLD",
			3,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1,
			48,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		},
		{
			"SPECULAR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			1,
			64,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		},
		{
			"SPECULARPOWER",
			0,
			DXGI_FORMAT_R32_FLOAT,
			1,
			80,
			D3D11_INPUT_PER_INSTANCE_DATA,
			1
		}
	};

	CreateInputLayout(inputElementDescriptions, GetDefaultTechniqueName());
}

//-------------------------------------------------------------------------

VertexBufferData InstancingPointLightMaterial::CreateVertexBufferData(
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
