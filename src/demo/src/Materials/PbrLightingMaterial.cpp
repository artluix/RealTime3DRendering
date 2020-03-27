#include "Materials/PbrLightingMaterial.h"

#include <library/Effect/Effect.h>
#include <library/Model/Mesh.h>

using namespace library;

PbrLightingMaterial::PbrLightingMaterial(Effect& effect, const std::string& defaultTechniqueName /*= "forward"*/)
	: LightsMaterial(effect, defaultTechniqueName)

	, m_ambientColor(effect.GetVariable("AmbientColor"))
	, m_cameraPosition(effect.GetVariable("CameraPosition"))

	, m_wvp(effect.GetVariable("WVP"))
	, m_world(effect.GetVariable("World"))
{
}
