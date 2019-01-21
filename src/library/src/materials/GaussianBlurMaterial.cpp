#include "StdAfx.h"
#include "library/materials/GaussianBlurMaterial.h"

#include "library/effect/Effect.h"

namespace library
{
	GaussianBlurMaterial::GaussianBlurMaterial(const Effect& effect)
		: PostProcessingMaterial(effect)
		, m_sampleOffsets(effect.GetVariable("sampleOffsets"))
		, m_sampleWeights(effect.GetVariable("sampleWeights"))
	{
	}

	GaussianBlurMaterial::~GaussianBlurMaterial() = default;
} // namespace library
