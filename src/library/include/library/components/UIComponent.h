#pragma once
#include "library/components/DrawableComponent.h"
#include "library/Math.h"

namespace library
{
	namespace components
	{
		class UIComponent : public rtti::Class<UIComponent, DrawableComponent>
		{
		public:
			explicit UIComponent(const Application& app);

			void SetPosition(const math::Vector2& position);
			const math::Vector2& GetPosition() const { return m_position; }

			math::Vector2 m_position;
		};
	} // namespace components
} // namespace library
