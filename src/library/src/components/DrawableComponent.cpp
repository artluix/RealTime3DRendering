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

	void DrawableComponent::SetAutoRenderable(const bool autoRenderable)
	{
		m_isAutoRenderable = autoRenderable;
	}

	void DrawableComponent::Update(const Time& time)
	{
		Component::Update(time);

		if (m_isAutoRenderable)
			AddToRenderer();
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
