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
		: ProjectorComponent()
	{
		m_fieldOfView = k_defaultFieldOfView;
		m_nearPlaneDistance = k_defaultNearPlaneDistance;
		m_farPlaneDistance = k_defaultFarPlaneDistance;
	}

	void CameraComponent::Initialize(const Application& app)
	{
		SetAspectRatio(app.GetAspectRatio());

		ProjectorComponent::Initialize(app);
	}

	void CameraComponent::Reset()
	{
		ProjectorComponent::Reset();

		SetAspectRatio(GetApp()->GetAspectRatio());
		SetFieldOfView(k_defaultFieldOfView);
		SetNearPlaneDistance(k_defaultNearPlaneDistance);
		SetFarPlaneDistance(k_defaultFarPlaneDistance);
	}
} // namespace library
