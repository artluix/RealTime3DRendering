#include "StdAfx.h"
#include "library/Components/CameraComponent.h"

#include "library/Application.h"

namespace library
{
	namespace
	{
		constexpr auto k_defaultFieldOfView = math::Pi_Div_4;
		constexpr auto k_defaultNearPlaneDistance = 0.01f;
		constexpr auto k_defaultFarPlaneDistance = 1000.f;
	}

	//-------------------------------------------------------------------------

	CameraComponent::CameraComponent()
	{
		m_fieldOfView = k_defaultFieldOfView;
		m_nearPlaneDistance = k_defaultNearPlaneDistance;
		m_farPlaneDistance = k_defaultFarPlaneDistance;
	}

	void CameraComponent::Initialize(const Application& app)
	{
		SetAspectRatio(app.GetAspectRatio());

		PerspectiveProjectorComponent::Initialize(app);
	}

	void CameraComponent::Reset()
	{
		PerspectiveProjectorComponent::Reset();

		m_aspectRatio = GetApp()->GetAspectRatio();
		m_fieldOfView = k_defaultFieldOfView;
		m_nearPlaneDistance = k_defaultNearPlaneDistance;
		m_farPlaneDistance = k_defaultFarPlaneDistance;
	}
} // namespace library
