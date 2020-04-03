#include "Materials/ShadowMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

ShadowMappingMaterial::ShadowMappingMaterial(Effect& effect)
	: PhongLightingMaterial(effect, "shadow_mapping")

	, m_projectiveTextureMatrix(effect.GetVariable("ProjectiveTextureMatrix"))
	, m_shadowMapTexture(effect.GetVariable("ShadowMapTexture"))
	, m_shadowMapSize(effect.GetVariable("ShadowMapSize"))

	, m_cameraPosition(effect.GetVariable("CameraPosition"))
	, m_wvp(effect.GetVariable("WVP"))
	, m_world(effect.GetVariable("World"))
	, m_colorTexture(effect.GetVariable("ColorTexture"))
{}

//-------------------------------------------------------------------------

void ShadowMappingMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), "shadow_mapping_manual_pcf");
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), "shadow_mapping_pcf");
}
