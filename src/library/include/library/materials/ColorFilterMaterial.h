#pragma once
#include "library/materials/PostProcessingMaterial.h"

namespace library
{
	class ColorFilterMaterial : public PostProcessingMaterial
	{
		RTTI_CLASS(ColorFilterMaterial, PostProcessingMaterial)

	public:
		explicit ColorFilterMaterial(const Effect& effect);
		~ColorFilterMaterial();

		EffectVariable& GetColorFilter() const { return m_colorFilter; }

	private:
		EffectVariable& m_colorFilter;
	};
} // namespace library
