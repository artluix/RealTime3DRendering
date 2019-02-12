#include "StdAfx.h"
#include "library/components/DrawableComponent.h"

#include "library/Application.h"
#include "library/Renderer.h"

namespace library
{
	void DrawableComponent::SetVisible(const bool visible)
	{
		m_visible = visible;
	}

	//-------------------------------------------------------------------------

	void DrawableComponent::AddToRenderer()
	{
		m_app->GetRenderer()->AddDrawable(this);
	}

	void DrawableComponent::RemoveFromRenderer()
	{
		m_app->GetRenderer()->RemoveDrawable(this);
	}
} // namespace library
