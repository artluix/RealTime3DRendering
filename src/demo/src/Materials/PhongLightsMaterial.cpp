#include "Materials/PhongLightsMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

PhongLightsMaterial::PhongLightsMaterial(Effect& effect)
	: PhongLightingMaterial(effect, "forward")

	, m_cameraPosition(effect.GetVariable("CameraPosition"))

	, m_wvp(effect.GetVariable("WVP"))
	, m_world(effect.GetVariable("World"))

	, m_colorTexture(effect.GetVariable("ColorTexture"))
{
}

void PhongLightsMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), "deferred", "geometry");
	CreateInputLayout(MakeArrayBuffer(VertexPositionTexture::ElementDescriptions), "deferred", "light");
}
