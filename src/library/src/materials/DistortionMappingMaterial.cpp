#include "StdAfx.h"
#include "library/materials/DistortionMappingMaterial.h"

#include "library/effect/Effect.h"
#include "library/effect/EffectVariable.h"

namespace library
{
	DistortionMappingMaterial::DistortionMappingMaterial(Effect& effect)
		: PostProcessingMaterial(effect)
		, m_distortionMapTexture(effect.GetVariable("DistortionMapTexture"))
		, m_displacementScale(effect.GetVariable("displacementScale"))
		, m_wvp(effect.GetVariable("wvp"))
	{
	}

	DistortionMappingMaterial::~DistortionMappingMaterial() = default;
} // namespace library
