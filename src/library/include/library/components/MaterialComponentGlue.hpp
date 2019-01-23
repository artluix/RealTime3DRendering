#pragma once
#include "library/components/MaterialComponent.h"
#include "library/components/ConcreteMaterialComponent.hpp"

namespace library
{
	template<class MaterialType>
	class MaterialComponentGlue
		: public MaterialComponent
		, public ConcreteMaterialComponent<MaterialType>
	{
	protected:
		using MaterialComponent::MaterialComponent;
	};
} // namespace library
