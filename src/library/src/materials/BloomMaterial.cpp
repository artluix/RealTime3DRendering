#include "StdAfx.h"
#include "library/Materials/BloomMaterial.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectVariable.h"

namespace library
{
BloomMaterial::BloomMaterial(Effect& effect)
	: PostProcessingMaterial(effect)
	, m_bloomTexture(effect.GetVariable("BloomTexture"))
	, m_bloomThreshold(effect.GetVariable("BloomThreshold"))
	, m_bloomIntensity(effect.GetVariable("BloomIntensity"))
	, m_bloomSaturation(effect.GetVariable("BloomSaturation"))
	, m_sceneIntensity(effect.GetVariable("SceneIntensity"))
	, m_sceneSaturation(effect.GetVariable("SceneSaturation"))
{}

BloomMaterial::~BloomMaterial() = default;
} // namespace library
