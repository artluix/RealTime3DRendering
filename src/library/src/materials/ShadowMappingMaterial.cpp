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
{}

//-------------------------------------------------------------------------

void ShadowMappingMaterial::InitializeInternal()
{
	PointLightMaterial::InitializeInternal();

	CreateInputLayout("shadow_mapping_manual_pcf");
	CreateInputLayout("shadow_mapping_pcf");
}
} // namespace library
