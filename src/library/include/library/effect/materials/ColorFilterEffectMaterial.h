#pragma once
#include "library/effect/materials/PostProcessingEffectMaterial.h"

namespace library
{
	class ColorFilterEffectMaterial : public PostProcessingEffectMaterial
	{
		RTTI_CLASS(ColorFilterEffectMaterial, PostProcessingEffectMaterial)

	public:
		explicit ColorFilterEffectMaterial(const Effect& effect);
		~ColorFilterEffectMaterial();

		//EffectVariable& GetColorFilter() const { return m_colorFilter; }

	private:
		//EffectVariable& m_colorFilter;
	};
} // namespace library
