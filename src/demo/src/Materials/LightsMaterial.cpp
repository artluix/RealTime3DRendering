#include "Materials/LightsMaterial.h"

#include <library/Effect/Effect.h>
#include <library/Model/Mesh.h>

using namespace library;

LightsMaterial::LightsMaterial(Effect& effect, const std::string& defaultTechniqueName)
	: Material(effect, defaultTechniqueName)

	, m_dirLights(effect.GetVariable("DirLights"))
	, m_dirLightsCount(effect.GetVariable("DirLightsCount"))

	, m_pointLights(effect.GetVariable("PointLights"))
	, m_pointLightsCount(effect.GetVariable("PointLightsCount"))

	, m_spotlights(effect.GetVariable("Spotlights"))
	, m_spotlightsCount(effect.GetVariable("SpotlightsCount"))
{
}
