#include "StdAfx.h"
#include "library/Materials/GaussianBlurMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
GaussianBlurMaterial::GaussianBlurMaterial(Effect& effect)
	: PostProcessingMaterial(effect)
	, m_sampleOffsets(effect.GetVariable("SampleOffsets"))
	, m_sampleWeights(effect.GetVariable("SampleWeights"))
{}

GaussianBlurMaterial::~GaussianBlurMaterial() = default;
} // namespace library
