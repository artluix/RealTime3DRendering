#include "Materials/DirectionalShadowMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

DirectionalShadowMappingMaterial::DirectionalShadowMappingMaterial(Effect& effect)
	: DirectionalLightMaterial(effect, "directional_shadow_mapping")

	, m_projectiveTextureMatrix(effect.GetVariable("projectiveTextureMatrix"))
	, m_shadowMapTexture(effect.GetVariable("ShadowMapTexture"))
	, m_shadowMapSize(effect.GetVariable("shadowMapSize"))
{}

//-------------------------------------------------------------------------

void DirectionalShadowMappingMaterial::InitializeInternal()
{
	DirectionalLightMaterial::InitializeInternal();

	const auto& inputElementDescriptions = GetCurrentPass().GetInputElementDescriptions();

	CreateInputLayout(inputElementDescriptions, "directional_shadow_mapping_manual_pcf");
	CreateInputLayout(inputElementDescriptions, "directional_shadow_mapping_pcf");
}
