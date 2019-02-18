#include "StdAfx.h"
#include "library/materials/BloomMaterial.h"

#include "library/effect/Effect.h"
#include "library/effect/EffectVariable.h"

namespace library
{
	BloomMaterial::BloomMaterial(const Effect& effect)
		: PostProcessingMaterial(effect)
		, m_bloomTexture(effect.GetVariable("BloomTexture"))
		, m_bloomThreshold(effect.GetVariable("bloomThreshold"))
		, m_bloomIntensity(effect.GetVariable("bloomIntensity"))
		, m_bloomSaturation(effect.GetVariable("bloomSaturation"))
		, m_sceneIntensity(effect.GetVariable("sceneIntensity"))
		, m_sceneSaturation(effect.GetVariable("sceneSaturation"))
	{
	}

	BloomMaterial::~BloomMaterial() = default;
} // namespace library
