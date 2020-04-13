#include "Materials/PbrLightsMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

PbrLightsMaterial::PbrLightsMaterial(Effect& effect, const std::string& defaultTechniqueName /*= "values"*/)
	: PbrLightingMaterial(effect, defaultTechniqueName)

	, m_cameraPosition(effect.GetVariable("CameraPosition"))

	, m_wvp(effect.GetVariable("WVP"))
	, m_world(effect.GetVariable("World"))

	, m_albedo(effect.GetVariable("Albedo"))
	, m_metallic(effect.GetVariable("Metallic"))
	, m_roughness(effect.GetVariable("Roughness"))
	, m_ao(effect.GetVariable("AO"))

	, m_albedoMap(effect.GetVariable("AlbedoMap"))
	, m_metallicMap(effect.GetVariable("MetallicMap"))
	, m_roughnessMap(effect.GetVariable("RoughnessMap"))
	, m_aoMap(effect.GetVariable("AOMap"))
{
}

void PbrLightsMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), "values");
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), "maps");
}
