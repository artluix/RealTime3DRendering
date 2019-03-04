#include "StdAfx.h"
#include "library/Materials/GaussianBlurMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
	GaussianBlurMaterial::GaussianBlurMaterial(Effect& effect)
		: PostProcessingMaterial(effect)
		, m_sampleOffsets(effect.GetVariable("sampleOffsets"))
		, m_sampleWeights(effect.GetVariable("sampleWeights"))
	{
	}

	GaussianBlurMaterial::~GaussianBlurMaterial() = default;
} // namespace library
