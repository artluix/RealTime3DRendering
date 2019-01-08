#include "StdAfx.h"
#include "library/components/UIComponent.h"

namespace library
{
	UIComponent::UIComponent(const Application& app)
		: DrawableComponent(app)
		, m_position(math::constants::Vector2::Zero)
	{
	}

	void UIComponent::SetPosition(const math::Vector2& position)
	{
		if (m_position != position)
		{
			m_position = position;
		}
	}

} // namespace library
