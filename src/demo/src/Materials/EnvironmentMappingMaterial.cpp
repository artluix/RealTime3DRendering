#include "Materials/EnvironmentMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

EnvironmentMappingMaterial::EnvironmentMappingMaterial(Effect& effect)
	: Material(effect, "main11")

	, m_ambientColor(effect.GetVariable("AmbientColor"))
	, m_environmentColor(effect.GetVariable("EnvColor"))
	, m_cameraPosition(effect.GetVariable("CameraPosition"))

	, m_wvp(effect.GetVariable("WVP"))
	, m_world(effect.GetVariable("World"))
	, m_reflectionAmount(effect.GetVariable("ReflectionAmount"))

	, m_colorTexture(effect.GetVariable("ColorTexture"))
	, m_environmentMap(effect.GetVariable("EnvironmentMap"))
{}

EnvironmentMappingMaterial::~EnvironmentMappingMaterial() = default;

void EnvironmentMappingMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
}
