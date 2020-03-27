#include "Materials/ShadowMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

ShadowMappingMaterial::ShadowMappingMaterial(Effect& effect)
	: PhongLightingMaterial(effect, "shadow_mapping")

	, m_projectiveTextureMatrix(effect.GetVariable("ProjectiveTextureMatrix"))
	, m_shadowMapTexture(effect.GetVariable("ShadowMapTexture"))
	, m_shadowMapSize(effect.GetVariable("ShadowMapSize"))
{}

//-------------------------------------------------------------------------

void ShadowMappingMaterial::InitializeInternal()
{
	PhongLightingMaterial::InitializeInternal();

	const auto& inputElementDescriptions = GetCurrentPass().GetInputElementDescriptions();

	CreateInputLayout(inputElementDescriptions, "shadow_mapping_manual_pcf");
	CreateInputLayout(inputElementDescriptions, "shadow_mapping_pcf");
}
