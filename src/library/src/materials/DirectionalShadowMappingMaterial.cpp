#include "StdAfx.h"
#include "library/Materials/DirectionalShadowMappingMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
	DirectionalShadowMappingMaterial::DirectionalShadowMappingMaterial(Effect& effect)
		: DirectionalLightMaterial(effect, "directional_shadow_mapping")
		
		, m_projectiveTextureMatrix(effect.GetVariable("projectiveTextureMatrix"))
		, m_shadowMapTexture(effect.GetVariable("ShadowMapTexture"))
		, m_shadowMapSize(effect.GetVariable("shadowMapSize"))
	{
	}

	//-------------------------------------------------------------------------

	void DirectionalShadowMappingMaterial::InitializeInternal()
	{
		DirectionalLightMaterial::InitializeInternal();

		CreateInputLayout("directional_shadow_mapping_manual_pcf");
		CreateInputLayout("directional_shadow_mapping_pcf");
	}
} // namespace library
