#include "StdAfx.h"
#include "library/Components/CameraComponent.h"

#include "library/Application.h"
#include "library/Math/Math.h"

namespace library
{

namespace
{
constexpr auto k_defaultFieldOfView = math::Pi_Div_4;
constexpr auto k_defaultNearPlaneDistance = 0.01f;
constexpr auto k_defaultFarPlaneDistance = 1000.f;
} // namespace

//-------------------------------------------------------------------------

CameraComponent::CameraComponent() : ProjectorComponent(ProjectionType::Perspective)
{
	m_fieldOfView = k_defaultFieldOfView;
	m_nearPlaneDistance = k_defaultNearPlaneDistance;
	m_farPlaneDistance = k_defaultFarPlaneDistance;
}

CameraComponent::CameraComponent(
	const float nearPlaneDistance,
	const float farPlaneDistance,
	const float fieldOfView,
	const float aspectRatio)
	: ProjectorComponent(nearPlaneDistance, farPlaneDistance, fieldOfView, aspectRatio)
{}

void CameraComponent::Initialize()
{
	m_aspectRatio = GetApp().GetAspectRatio();

	ProjectorComponent::Initialize();
}
} // namespace library
