#include "StdAfx.h"
#include "library/materials/PostProcessingMaterial.h"

#include "library/effect/Effect.h"
#include "library/effect/EffectTechnique.h"
#include "library/effect/EffectPass.h"

#include "library/Mesh.h"

namespace library
{
	PostProcessingMaterial::PostProcessingMaterial(const Effect& effect)
		: Material(effect, "main11")
		, m_colorTexture(effect.GetVariable("ColorTexture"))
	{
	}

	PostProcessingMaterial::~PostProcessingMaterial() = default;

	void PostProcessingMaterial::InitializeInternal()
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptions =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		for (unsigned i = 0, techniquesCount = m_effect.GetTechniquesCount(); i < techniquesCount; i++)
		{
			const auto& technique = m_effect.GetTechnique(i);

			for (unsigned j = 0, passesCount = technique.GetPassesCount(); j < passesCount; j++)
			{
				const auto& pass = technique.GetPass(j);
				CreateInputLayout(pass, inputElementDescriptions);
			}
		}
	}

	ComPtr<ID3D11Buffer> PostProcessingMaterial::CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const
	{
		if (!mesh.HasVertices())
			return ComPtr<ID3D11Buffer>();

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
				DirectX::XMFLOAT4(position.x, position.y, position.z, 1.0f),
				DirectX::XMFLOAT2(uv.x, uv.y)
			);
		}

		return Material::CreateVertexBuffer(device, vertices.data(), vertices.size() * sizeof(Vertex));
	}
} // namespace library
