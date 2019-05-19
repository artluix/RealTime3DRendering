#pragma once
#include "library/Materials/TessellationMaterial.h"

namespace library
{
	class TriangleTessellationMaterial : public TessellationMaterial
	{
		RTTI_CLASS(TriangleTessellationMaterial, TessellationMaterial)

	public:
		explicit TriangleTessellationMaterial(Effect& effect);

		EffectVariable& GetTessellationInsideFactor() { return m_tessellationInsideFactor; }

	private:
		EffectVariable& m_tessellationInsideFactor;
	};
} // namespace library
