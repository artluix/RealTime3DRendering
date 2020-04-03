#include "Materials/SkinnedModelMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

SkinnedModelMaterial::SkinnedModelMaterial(Effect& effect)
	: PhongLightingMaterial(effect, "main11")

	, m_boneTransforms(effect.GetVariable("BoneTransforms"))

	, m_cameraPosition(effect.GetVariable("CameraPosition"))
	, m_wvp(effect.GetVariable("WVP"))
	, m_world(effect.GetVariable("World"))
	, m_colorTexture(effect.GetVariable("ColorTexture"))
{}

SkinnedModelMaterial::~SkinnedModelMaterial() = default;

//-------------------------------------------------------------------------

void SkinnedModelMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
}
