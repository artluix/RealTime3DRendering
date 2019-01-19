#include "StdAfx.h"
#include "library/effect/materials/ColorFilterEffectMaterial.h"

#include "library/effect/Effect.h"

namespace library
{
	std::string ColorFilter::ToTechniqueName(const Type t)
	{
		switch (t)
		{
			case Grayscale:		return "grayscale_filter";
			case Inverse:		return "inverse_filter";
			case Sepia:			return "sepia_filter";
			case Generic:		return "generic_filter";
			default:			return "";
		}
	}

	std::string ColorFilter::ToString(const Type t)
	{
		switch (t)
		{
			case Original:		return "Original";
			case Grayscale:		return "Grayscale";
			case Inverse:		return "Inverse";
			case Sepia:			return "Sepia";
			case Generic:		return "Generic";
			default:			return "Error";
		}
	}

	//-------------------------------------------------------------------------

	ColorFilterEffectMaterial::ColorFilterEffectMaterial(const Effect& effect)
		: PostProcessingEffectMaterial(effect)
		, m_colorFilter(effect.GetVariable("colorFilter"))
	{
	}

	ColorFilterEffectMaterial::~ColorFilterEffectMaterial() = default;
} // namespace library
