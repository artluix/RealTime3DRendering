#include "Materials/FogMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

FogMaterial::FogMaterial(Effect& effect)
	: FogMaterial(effect, "fogEnabled")
{}

FogMaterial::FogMaterial(library::Effect& effect, const std::string& defaultTechniqueName)
	: PhongLightingMaterial(effect, defaultTechniqueName)

	, m_fogColor(effect.GetVariable("FogColor"))
	, m_fogStart(effect.GetVariable("FogStart"))
	, m_fogRange(effect.GetVariable("FogRange"))

	, m_cameraPosition(effect.GetVariable("CameraPosition"))
	, m_wvp(effect.GetVariable("WVP"))
	, m_world(effect.GetVariable("World"))
	, m_colorTexture(effect.GetVariable("ColorTexture"))
{
}

FogMaterial::~FogMaterial() = default;

//-------------------------------------------------------------------------

void FogMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), "fogDisabled");
}
