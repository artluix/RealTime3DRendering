#include "StdAfx.h"
#include "library/materials/DiffuseLightingMaterial.h"

#include "library/effect/Effect.h"
#include "library/Mesh.h"

#include <cassert>

namespace library
{
	DiffuseLightingMaterial::DiffuseLightingMaterial()
		: Material()
	{
	}

	DiffuseLightingMaterial::~DiffuseLightingMaterial() = default;

	void DiffuseLightingMaterial::Initialize(const Effect& effect)
	{
		Material::Initialize(effect);

		m_wvp = effect.GetVariable("wvp");
		assert(!!m_wvp);

		m_world = effect.GetVariable("world");
		assert(!!m_world);

		m_ambientColor = effect.GetVariable("ambientColor");
		assert(!!m_ambientColor);

		m_lightColor = effect.GetVariable("lightColor");
		assert(!!m_lightColor);

		m_lightDirection = effect.GetVariable("lightDirection");
		assert(!!m_lightDirection);

		m_colorTexture = effect.GetVariable("ColorTexture");
		assert(!!m_colorTexture);

		std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementDescriptions =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		CreateInputLayout("main10", "p0", inputElementDescriptions);
	}

	ComPtr<ID3D11Buffer> DiffuseLightingMaterial::CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const
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

		return Material::CreateVertexBuffer(device, vertices.data(), vertices.size() * sizeof(Vertex));
	}

} // namespace library
