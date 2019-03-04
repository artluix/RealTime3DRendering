#include "StdAfx.h"
#include "library/Materials/TextureMappingMaterial.h"

#include "library/Effect/Effect.h"
#include "library/Mesh.h"

namespace library
{
	TextureMappingMaterial::TextureMappingMaterial(Effect& effect)
		: Material(effect, "main10")
		, m_wvp(effect.GetVariable("wvp"))
		, m_colorTexture(effect.GetVariable("ColorTexture"))
	{
	}

	TextureMappingMaterial::~TextureMappingMaterial() = default;

	void TextureMappingMaterial::InitializeInternal()
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptions =
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

		CreateInputLayout(inputElementDescriptions, "main10");
	}

	ComPtr<ID3D11Buffer> TextureMappingMaterial::CreateVertexBuffer(
		ID3D11Device* const device,
		const Mesh& mesh
	) const
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

		return Material::CreateVertexBuffer(device, vertices);
	}
} // namespace library
