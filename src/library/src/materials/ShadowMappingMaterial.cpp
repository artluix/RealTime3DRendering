#include "StdAfx.h"
#include "library/Materials/ShadowMappingMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
	ShadowMappingMaterial::ShadowMappingMaterial(Effect& effect)
		: PointLightMaterial(effect, "shadow_mapping")
		, m_projectiveTextureMatrix(effect.GetVariable("projectiveTextureMatrix"))
		, m_shadowMapTexture(effect.GetVariable("ShadowMapTexture"))
		, m_shadowMapSize(effect.GetVariable("shadowMapSize"))
	{
	}

	void ShadowMappingMaterial::InitializeInternal()
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

		CreateInputLayout(inputElementDescriptions, "shadow_mapping");
		CreateInputLayout(inputElementDescriptions, "shadow_mapping_manual_pcf");
		CreateInputLayout(inputElementDescriptions, "shadow_mapping_pcf");
	}
} // namespace library
