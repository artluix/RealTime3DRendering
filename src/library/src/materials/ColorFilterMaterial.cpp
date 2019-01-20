#include "StdAfx.h"
#include "library/materials/ColorFilterMaterial.h"

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

	ColorFilterMaterial::ColorFilterMaterial(const Effect& effect)
		: PostProcessingMaterial(effect)
		, m_colorFilter(effect.GetVariable("colorFilter"))
	{
	}

	ColorFilterMaterial::~ColorFilterMaterial() = default;
} // namespace library
