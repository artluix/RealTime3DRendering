#include "StdAfx.h"
#include "library/Materials/ColorFilterMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
	ColorFilterMaterial::ColorFilterMaterial(Effect& effect)
		: PostProcessingMaterial(effect)
		, m_colorFilter(effect.GetVariable("colorFilter"))
	{
	}

	ColorFilterMaterial::~ColorFilterMaterial() = default;
} // namespace library
