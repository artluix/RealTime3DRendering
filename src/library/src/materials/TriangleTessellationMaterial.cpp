#include "StdAfx.h"
#include "library/Materials/TriangleTessellationMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
	TriangleTessellationMaterial::TriangleTessellationMaterial(Effect& effect)
		: TessellationMaterial(effect)

		, m_tessellationInsideFactor(effect.GetVariable("k_tessellationInsideFactor"))
	{
	}
} // namespace library
