#pragma once
#include "library/components/DrawableComponent.h"

namespace library
{
	class Material;

	class DrawableMaterialComponent : public virtual DrawableComponent
	{
		RTTI_CLASS(DrawableMaterialComponent, DrawableComponent)

	public:
		using DrawableComponent::Initialize;
		using DrawableComponent::Update;
		using DrawableComponent::Draw;

		virtual const Material& GetMaterial() const = 0;

	protected:
		explicit DrawableMaterialComponent() = default;

		virtual Material& GetMaterial() = 0;
	};
} // namespace library
