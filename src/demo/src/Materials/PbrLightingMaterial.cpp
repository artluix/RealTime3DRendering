#include "Materials/PbrLightingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

PbrLightingMaterial::PbrLightingMaterial(Effect& effect, const std::string& defaultTechniqueName)
	: LightsMaterial(effect, defaultTechniqueName)

	, m_ambientColor(effect.GetVariable("AmbientColor"))
{
}
