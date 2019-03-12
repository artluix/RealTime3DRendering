#include "StdAfx.h"
#include "library/Components/PerspectiveProjectorComponent.h"

namespace library
{
	namespace
	{
		constexpr auto k_defaultFieldOfView = math::Pi_Div_4;
		constexpr auto k_defaultAspectRatio = 4.f / 3.f;
		constexpr auto k_defaultNearPlaneDistance = 0.5f;
		constexpr auto k_defaultFarPlaneDistance = 100.f;
	}

	//-------------------------------------------------------------------------

	PerspectiveProjectorComponent::PerspectiveProjectorComponent()
		: PerspectiveProjectorComponent(
			k_defaultFieldOfView,
			k_defaultAspectRatio,
			k_defaultNearPlaneDistance, k_defaultFarPlaneDistance
		)
	{
	}

	PerspectiveProjectorComponent::PerspectiveProjectorComponent(
		const float fieldOfView,
		const float aspectRatio,
		const float nearPlaneDistance,
		const float farPlaneDistance
	)
		: ProjectorComponent(nearPlaneDistance, farPlaneDistance)
		, m_fieldOfView(fieldOfView)
		, m_aspectRatio(aspectRatio)
	{
	}

	//-------------------------------------------------------------------------

	void PerspectiveProjectorComponent::SetFieldOfView(const float fieldOfView)
	{
		if (m_fieldOfView != fieldOfView)
		{
			m_fieldOfView = fieldOfView;
			m_isProjectionMatrixDirty = true;
		}
	}

	void PerspectiveProjectorComponent::SetAspectRatio(const float aspectRatio)
	{
		if (m_aspectRatio != aspectRatio)
		{
			m_aspectRatio = aspectRatio;
			m_isProjectionMatrixDirty = true;
		}
	}

	//-------------------------------------------------------------------------

	bool PerspectiveProjectorComponent::UpdateProjectionMatrix()
	{
		if (!m_isProjectionMatrixDirty)
			return false;

		m_projectionMatrix = math::Matrix4::PerspectiveFovRH(
			m_fieldOfView,
			m_aspectRatio,
			m_nearPlaneDistance,
			m_farPlaneDistance
		);
		m_isProjectionMatrixDirty = false;

		return true;
	}

	void PerspectiveProjectorComponent::Reset()
	{
		ProjectorComponent::Reset();

		m_fieldOfView = k_defaultFieldOfView;
		m_aspectRatio = k_defaultAspectRatio;
	}
} // namespace library
