#include "StdAfx.h"
#include "library/materials/GaussianBlurMaterial.h"

#include "library/effect/Effect.h"

namespace library
{
	GaussianBlurMaterial::GaussianBlurMaterial(const Effect& effect)
		: PostProcessingMaterial(effect)
		, m_samples(effect.GetVariable("samples"))
	{
	}

	GaussianBlurMaterial::~GaussianBlurMaterial() = default;
} // namespace library
