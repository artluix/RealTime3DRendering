#include "Materials/DisplacementMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

DisplacementMappingMaterial::DisplacementMappingMaterial(Effect& effect)
	: PhongLightingMaterial(effect, "main11")

	, m_displacementScale(effect.GetVariable("DisplacementScale"))
	, m_displacementMap(effect.GetVariable("DisplacementMap"))

	, m_cameraPosition(effect.GetVariable("CameraPosition"))
	, m_wvp(effect.GetVariable("WVP"))
	, m_world(effect.GetVariable("World"))
	, m_colorTexture(effect.GetVariable("ColorTexture"))
{}

DisplacementMappingMaterial::~DisplacementMappingMaterial() = default;

//-------------------------------------------------------------------------

void DisplacementMappingMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
}
