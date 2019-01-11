#pragma once
#include "library/components/DrawableComponent.h"
#include "library/math/Math.h"

namespace library
{
	class UIComponent : public DrawableComponent
	{
		RTTI_CLASS(UIComponent, DrawableComponent)

	public:
		explicit UIComponent(const Application& app);
		~UIComponent() = default;

		void SetPosition(const math::Vector2& position);
		const math::Vector2& GetPosition() const { return m_position; }

	protected:
		math::Vector2 m_position;
	};
} // namespace library
