#include "StdAfx.h"
#include "library/Materials/ProjectiveTextureMappingMaterial.h"

#include "library/Effect/Effect.h"
#include "library/Mesh.h"

namespace library
{
	ProjectiveTextureMappingMaterial::ProjectiveTextureMappingMaterial(Effect& effect)
		: PointLightMaterial(effect, "project_texture")
		, m_projectedTexture(effect.GetVariable("ProjectedTexture"))
		, m_projectiveTextureMatrix(effect.GetVariable("projectiveTextureMatrix"))
		, m_depthMapTextureMatrix(effect.GetVariable("DepthMapTexture"))
		, m_depthBias(effect.GetVariable("depthBias"))
	{
	}

	void ProjectiveTextureMappingMaterial::InitializeInternal()
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

		CreateInputLayout(inputElementDescriptions, "project_texture");
		CreateInputLayout(inputElementDescriptions, "project_texture_depth_map");
	}
} // namespace library
