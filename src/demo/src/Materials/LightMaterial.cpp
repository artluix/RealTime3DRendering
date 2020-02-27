#include "Materials/LightMaterial.h"

#include <library/Effect/Effect.h>
#include <library/Model/Mesh.h>

using namespace library;

LightMaterial::LightMaterial(Effect& effect, const std::string& defaultTechniqueName /*= "main11"*/)
	: Material(effect, defaultTechniqueName)

	, m_ambientColor(effect.GetVariable("AmbientColor"))
	, m_cameraPosition(effect.GetVariable("CameraPosition"))

	, m_lightsData(effect.GetVariable("LightsData"))

	, m_wvp(effect.GetVariable("WVP"))
	, m_world(effect.GetVariable("World"))

	, m_specularPower(effect.GetVariable("SpecularPower"))
	, m_specularColor(effect.GetVariable("SpecularColor"))

	, m_colorTexture(effect.GetVariable("ColorTexture"))
{}

LightMaterial::~LightMaterial() = default;

//-------------------------------------------------------------------------

void LightMaterial::InitializeInternal()
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

	CreateInputLayout(inputElementDescriptions, GetDefaultTechniqueName());
}

VertexBufferData LightMaterial::CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const
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
