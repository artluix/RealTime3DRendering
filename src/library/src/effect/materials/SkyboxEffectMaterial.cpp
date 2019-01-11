#include "StdAfx.h"
#include "library/effect/materials/SkyboxEffectMaterial.h"

#include "library/effect/Effect.h"
#include "library/Mesh.h"
#include "library/Color.h"

namespace library
{
	SkyboxEffectMaterial::SkyboxEffectMaterial(const Effect& effect)
		: EffectMaterial(effect, "main11")
		, m_worldViewProjection(effect.GetVariable("WorldViewProjection"))
		, m_skyboxTexture(effect.GetVariable("SkyboxTexture"))
	{
	}

	SkyboxEffectMaterial::~SkyboxEffectMaterial() = default;

	void SkyboxEffectMaterial::InitializeInternal()
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptions =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		CreateInputLayout("main11", "p0", inputElementDescriptions);
	}

	ComPtr<ID3D11Buffer> SkyboxEffectMaterial::CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const
	{
		if (!mesh.HasVertices())
			return ComPtr<ID3D11Buffer>();

		std::vector<Vertex> vertices;

		const auto& meshVertices = mesh.GetVertices();
		const auto verticesCount = meshVertices.size();

		vertices.reserve(verticesCount);

		for (unsigned i = 0; i < verticesCount; i++)
		{
			const auto& position = meshVertices[i];
			vertices.emplace_back(DirectX::XMFLOAT4(position.x, position.y, position.z, 1.0f));
		}

		return EffectMaterial::CreateVertexBuffer(device, vertices.data(), vertices.size() * sizeof(Vertex));
	}
} // namespace library
