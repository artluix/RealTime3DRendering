#pragma once
#include "library/components/DrawableComponent.h"

namespace library
{
	class Material;

	class MaterialDrawableComponent : public virtual DrawableComponent
	{
		RTTI_CLASS(MaterialDrawableComponent, DrawableComponent)

	public:
		virtual const Material& GetMaterial() const = 0;

	protected:
		explicit MaterialDrawableComponent() = default;

		virtual Material& GetMaterial() = 0;
	};
} // namespace library
