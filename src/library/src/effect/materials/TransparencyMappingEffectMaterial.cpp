#include "StdAfx.h"
#include "library/effect/materials/TransparencyMappingEffectMaterial.h"

#include "library/effect/Effect.h"
#include "library/Mesh.h"

namespace library
{
	TransparencyMappingEffectMaterial::TransparencyMappingEffectMaterial(const Effect& effect)
		: EffectMaterial(effect, "alphaBlendingWithoutFog")

		, m_ambientColor(effect.GetVariable("ambientColor"))
		, m_lightColor(effect.GetVariable("lightColor"))
		, m_lightPosition(effect.GetVariable("lightPosition"))
		, m_lightRadius(effect.GetVariable("lightRadius"))
		, m_fogColor(effect.GetVariable("fogColor"))
		, m_fogStart(effect.GetVariable("fogStart"))
		, m_fogRange(effect.GetVariable("fogRange"))
		, m_cameraPosition(effect.GetVariable("cameraPosition"))

		, m_wvp(effect.GetVariable("wvp"))
		, m_world(effect.GetVariable("world"))
		, m_specularColor(effect.GetVariable("specularColor"))
		, m_specularPower(effect.GetVariable("specularPower"))
		
		, m_colorTexture(effect.GetVariable("ColorTexture"))
		, m_transparencyMap(effect.GetVariable("TransparencyMap"))
	{
	}

	TransparencyMappingEffectMaterial::~TransparencyMappingEffectMaterial() = default;

	void TransparencyMappingEffectMaterial::InitializeInternal()
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptions =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		CreateInputLayout("alphaBlendingWithoutFog", "p0", inputElementDescriptions);
		CreateInputLayout("alphaBlendingWithFog", "p0", inputElementDescriptions);
	}

	ComPtr<ID3D11Buffer> TransparencyMappingEffectMaterial::CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const
	{
		if (!mesh.HasVertices())
			return ComPtr<ID3D11Buffer>();

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
				DirectX::XMFLOAT4(position.x, position.y, position.z, 1.0f),
				DirectX::XMFLOAT2(uv.x, uv.y),
				normal
			);
		}

		return EffectMaterial::CreateVertexBuffer(device, vertices.data(), vertices.size() * sizeof(Vertex));
	}
} // namespace library
