#include "Materials/DiffuseLightingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

DiffuseLightingMaterial::DiffuseLightingMaterial(Effect& effect)
	: Material(effect, "main11")

	, m_ambientColor(effect.GetVariable("AmbientColor"))
	, m_lightData(effect.GetVariable("LightData"))

	, m_wvp(effect.GetVariable("WVP"))
	, m_world(effect.GetVariable("World"))

	, m_colorTexture(effect.GetVariable("ColorTexture"))
{}

DiffuseLightingMaterial::~DiffuseLightingMaterial() = default;

//-------------------------------------------------------------------------

void DiffuseLightingMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
}
