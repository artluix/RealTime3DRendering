#pragma once
#include "library/Materials/TessellationMaterial.h"

namespace library
{
	class QuadTessellationMaterial : public TessellationMaterial
	{
		RTTI_CLASS(QuadTessellationMaterial, TessellationMaterial)

	public:
		explicit QuadTessellationMaterial(Effect& effect);

		EffectVariable& GetTessellationInsideFactors() { return m_tessellationInsideFactors; }

	private:
		EffectVariable& m_tessellationInsideFactors;
	};
} // namespace library
