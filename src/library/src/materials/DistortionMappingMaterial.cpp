#include "StdAfx.h"
#include "library/Materials/DistortionMappingMaterial.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectVariable.h"

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
