#pragma once
#include "library/effect/materials/PostProcessingEffectMaterial.h"

namespace library
{
	struct ColorFilter
	{
		enum Type : unsigned
		{
			Original = 0, // without color changes
			Grayscale,
			Inverse,
			Sepia,
			Generic,

			//# Count
			Count,
		};

		static std::string ToTechniqueName(const Type t);
		static std::string ToString(const Type t);
	};

	class ColorFilterEffectMaterial : public PostProcessingEffectMaterial
	{
		RTTI_CLASS(ColorFilterEffectMaterial, PostProcessingEffectMaterial)

	public:
		explicit ColorFilterEffectMaterial(const Effect& effect);
		~ColorFilterEffectMaterial();

		EffectVariable& GetColorFilter() const { return m_colorFilter; }

	private:
		EffectVariable& m_colorFilter;
	};
} // namespace library
