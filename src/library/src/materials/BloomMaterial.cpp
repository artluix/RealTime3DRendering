#include "StdAfx.h"
#include "library/Materials/BloomMaterial.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectVariable.h"

namespace library
{
BloomMaterial::BloomMaterial(Effect& effect)
	: PostProcessingMaterial(effect)
	, m_bloomTexture(effect.GetVariable("BloomTexture"))
	, m_bloomThreshold(effect.GetVariable("bloomThreshold"))
	, m_bloomIntensity(effect.GetVariable("bloomIntensity"))
	, m_bloomSaturation(effect.GetVariable("bloomSaturation"))
	, m_sceneIntensity(effect.GetVariable("sceneIntensity"))
	, m_sceneSaturation(effect.GetVariable("sceneSaturation"))
{}

BloomMaterial::~BloomMaterial() = default;
} // namespace library
