#include "Materials/ShadowMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

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

	const auto& inputElementDescriptions = GetCurrentPass().GetInputElementDescriptions();

	CreateInputLayout(inputElementDescriptions, "shadow_mapping_manual_pcf");
	CreateInputLayout(inputElementDescriptions, "shadow_mapping_pcf");
}
