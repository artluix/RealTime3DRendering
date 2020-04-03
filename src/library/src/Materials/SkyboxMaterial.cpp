#include "StdAfx.h"
#include "library/Materials/SkyboxMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
SkyboxMaterial::SkyboxMaterial(Effect& effect)
	: Material(effect, "main11")
	, m_worldViewProjection(effect.GetVariable("WorldViewProjection"))
	, m_skyboxTexture(effect.GetVariable("SkyboxTexture"))
{}

SkyboxMaterial::~SkyboxMaterial() = default;

//-------------------------------------------------------------------------

void SkyboxMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
}
} // namespace library
