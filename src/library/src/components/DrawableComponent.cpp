#include "StdAfx.h"
#include "library/components/DrawableComponent.h"

namespace library
{
	DrawableComponent::DrawableComponent(const Application& app)
		: Class(app)
		, m_visible(true)
	{
	}

	void DrawableComponent::SetVisible(const bool visible)
	{
		m_visible = visible;
	}

	void DrawableComponent::Draw(const Time& time)
	{
	}

} // namespace library
