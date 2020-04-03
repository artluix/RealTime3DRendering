#include "Materials/NormalMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

NormalMappingMaterial::NormalMappingMaterial(Effect& effect)
	: PhongLightingMaterial(effect, "main11")

	, m_normalMap(effect.GetVariable("NormalMap"))

	, m_cameraPosition(effect.GetVariable("CameraPosition"))
	, m_wvp(effect.GetVariable("WVP"))
	, m_world(effect.GetVariable("World"))
	, m_colorTexture(effect.GetVariable("ColorTexture"))
{}

NormalMappingMaterial::~NormalMappingMaterial() = default;

//-------------------------------------------------------------------------

void NormalMappingMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
}
