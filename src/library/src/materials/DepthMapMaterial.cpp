#include "StdAfx.h"
#include "library/Materials/DepthMapMaterial.h"

#include "library/Effect/Effect.h"

#include "library/Model/Model.h"
#include "library/Model/Mesh.h"

namespace library
{
	DepthMapMaterial::DepthMapMaterial(Effect& effect)
		: Material(effect, "create_depth_map")
		, m_worldLightViewProjection(effect.GetVariable("worldLightViewProjection"))
	{
	}

	DepthMapMaterial::~DepthMapMaterial() = default;

	//-------------------------------------------------------------------------

	void DepthMapMaterial::InitializeInternal()
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
			}
		};

		Material::InitializeInternal();

		// CreateInputLayout("depth_map_render_target");
		CreateInputLayout("depth_map_bias");
	}
	
	ComPtr<ID3D11Buffer> DepthMapMaterial::CreateVertexBuffer(
		ID3D11Device* const device,
		const Mesh& mesh
	) const
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

		return Material::CreateVertexBuffer(device, vertices);
	}
} // namespace library
