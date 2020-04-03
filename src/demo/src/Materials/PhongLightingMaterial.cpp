#include "Materials/PhongLightingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

PhongLightingMaterial::PhongLightingMaterial(Effect& effect, const std::string& defaultTechniqueName)
	: LightsMaterial(effect, defaultTechniqueName)

	, m_ambientColor(effect.GetVariable("AmbientColor"))

	, m_specularPower(effect.GetVariable("SpecularPower"))
	, m_specularColor(effect.GetVariable("SpecularColor"))

	// deferred
	, m_colorBufferTexture(effect.GetVariable("ColorBufferTexture"))
	, m_normalBufferTexture(effect.GetVariable("NormalBufferTexture"))
	, m_worldPositionBufferTexture(effect.GetVariable("WorldPositionBufferTexture"))
	, m_depthBufferTexture(effect.GetVariable("DepthBufferTexture"))
{
}
