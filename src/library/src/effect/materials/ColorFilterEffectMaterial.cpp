#include "StdAfx.h"
#include "library/effect/materials/ColorFilterEffectMaterial.h"

#include "library/effect/Effect.h"

namespace library
{
	ColorFilterEffectMaterial::ColorFilterEffectMaterial(const Effect& effect)
		: PostProcessingEffectMaterial(effect)
		//, m_colorFilter(effect.GetVariable("colorFilter"))
	{
	}

	ColorFilterEffectMaterial::~ColorFilterEffectMaterial() = default;
} // namespace library
