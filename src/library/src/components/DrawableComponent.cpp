#include "StdAfx.h"
#include "library/components/DrawableComponent.h"

namespace library
{
	DrawableComponent::DrawableComponent(const Application& app)
		: Component(app)
	{
	}

	DrawableComponent::DrawableComponent(const Application& app, const CameraComponent& camera)
		: Component(app)
		, m_camera(&camera)
	{
	}

	void DrawableComponent::SetVisible(const bool visible)
	{
		m_visible = visible;
	}

	void DrawableComponent::SetCamera(const CameraComponent& camera)
	{
		if (m_camera != &camera)
		{
			m_camera = &camera;
		}
	}

} // namespace library
