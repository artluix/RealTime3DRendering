#include "StdAfx.h"
#include "library/components/DrawableComponent.h"

namespace library
{
	void DrawableComponent::SetVisible(const bool visible)
	{
		m_visible = visible;
	}

	void DrawableComponent::SetCamera(const CameraComponent& camera)
	{
		m_camera = &camera;
	}
} // namespace library
