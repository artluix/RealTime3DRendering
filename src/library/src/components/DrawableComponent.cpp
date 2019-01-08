#include "StdAfx.h"
#include "library/components/DrawableComponent.h"

namespace library
{
	DrawableComponent::DrawableComponent(const Application& app)
		: Component(app)
	{
	}

	void DrawableComponent::SetVisible(const bool visible)
	{
		m_visible = visible;
	}

} // namespace library
