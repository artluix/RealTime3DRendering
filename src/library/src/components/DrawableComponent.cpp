#include "StdAfx.h"
#include "library/components/DrawableComponent.h"

#include "library/Application.h"
#include "library/Renderer.h"

namespace library
{
	DrawableComponent::DrawableComponent(const Application& app)
		: Component(app)
	{
	}

	void DrawableComponent::Initialize()
	{
		m_app.GetRenderer()->RegisterForRender(this);
	}

	void DrawableComponent::SetVisible(const bool visible)
	{
		m_visible = visible;
	}
} // namespace library
