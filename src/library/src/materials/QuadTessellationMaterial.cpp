#include "StdAfx.h"
#include "library/Materials/QuadTessellationMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
	QuadTessellationMaterial::QuadTessellationMaterial(Effect& effect)
		: TessellationMaterial(effect)

		, m_tessellationInsideFactors(effect.GetVariable("k_tessellationInsideFactors"))
	{
	}
} // namespace library
