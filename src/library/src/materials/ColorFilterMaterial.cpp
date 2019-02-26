#include "StdAfx.h"
#include "library/materials/ColorFilterMaterial.h"

#include "library/effect/Effect.h"

namespace library
{
	ColorFilterMaterial::ColorFilterMaterial(Effect& effect)
		: PostProcessingMaterial(effect)
		, m_colorFilter(effect.GetVariable("colorFilter"))
	{
	}

	ColorFilterMaterial::~ColorFilterMaterial() = default;
} // namespace library
