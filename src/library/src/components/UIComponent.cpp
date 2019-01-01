#include "StdAfx.h"
#include "library/components/UIComponent.h"

namespace library
{
	UIComponent::UIComponent(const Application& app)
		: Class(app)
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
