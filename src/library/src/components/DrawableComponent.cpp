#include "StdAfx.h"
#include "library/components/DrawableComponent.h"

#include "library/Application.h"
#include "library/Renderer.h"

namespace library
{
	void DrawableComponent::Initialize(const Application& app)
	{
		Component::Initialize(app);

		app.GetRenderer()->RegisterForRender(this);
	}

	void DrawableComponent::SetVisible(const bool visible)
	{
		m_visible = visible;
	}
} // namespace library
