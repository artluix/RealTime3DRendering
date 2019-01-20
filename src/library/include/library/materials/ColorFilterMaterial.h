#pragma once
#include "library/materials/PostProcessingMaterial.h"

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
