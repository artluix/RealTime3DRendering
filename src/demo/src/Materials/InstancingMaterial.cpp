#include "Materials/InstancingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

InstancingMaterial::InstancingMaterial(Effect& effect)
	: LightsMaterial(effect, "main11")

	, m_viewProjection(effect.GetVariable("viewProjection"))

	, m_ambientColor(effect.GetVariable("AmbientColor"))

	, m_cameraPosition(effect.GetVariable("CameraPosition"))
	, m_wvp(effect.GetVariable("WVP"))
	, m_world(effect.GetVariable("World"))
	, m_colorTexture(effect.GetVariable("ColorTexture"))
{
}

InstancingMaterial::~InstancingMaterial() = default;

//-------------------------------------------------------------------------

void InstancingMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(VertexInstance::ElementDescriptions), GetDefaultTechniqueName());
}
