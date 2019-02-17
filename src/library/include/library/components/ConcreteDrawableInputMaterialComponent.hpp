#pragma once
#include "library/components/ConcreteDrawableMaterialComponent.hpp"
#include "library/components/DrawableInputMaterialComponent.h"

namespace library
{
	template<class MaterialType>
	class ConcreteDrawableInputMaterialComponent
		: public virtual DrawableInputMaterialComponent
		, public ConcreteDrawableMaterialComponent<MaterialType>
	{
	protected:
		explicit ConcreteDrawableInputMaterialComponent() = default;
	};
} // namespace library
