#include "StdAfx.h"
#include "library/components/ProjectorComponent.h"

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

	ProjectorComponent::ProjectorComponent()
		: ProjectorComponent(
			k_defaultFieldOfView,
			k_defaultAspectRatio,
			k_defaultNearPlaneDistance,
			k_defaultFarPlaneDistance
		)
	{
	}

	ProjectorComponent::ProjectorComponent(
		const float fieldOfView,
		const float aspectRatio,
		const float nearPlaneDistance,
		const float farPlaneDistance
	)
		: m_position(math::Vector3::Zero)

		, m_direction(math::Vector3::Forward)
		, m_up(math::Vector3::Up)
		, m_right(math::Vector3::Right)

		, m_fieldOfView(fieldOfView)
		, m_aspectRatio(aspectRatio)
		, m_nearPlaneDistance(nearPlaneDistance)
		, m_farPlaneDistance(farPlaneDistance)

		, m_viewMatrix(math::Matrix4::Identity)
		, m_projectionMatrix(math::Matrix4::Identity)
		, m_viewProjectionMatrix(math::Matrix4::Identity)
	{
	}

	ProjectorComponent::~ProjectorComponent() = default;

	//-------------------------------------------------------------------------

	void ProjectorComponent::SetPosition(const math::Vector3& position)
	{
		if (m_position != position)
		{
			m_position = position;
			m_isViewMatrixDirty = true;
		}
	}

	void ProjectorComponent::SetAspectRatio(const float aspectRatio)
	{
		if (m_aspectRatio != aspectRatio)
		{
			m_aspectRatio = aspectRatio;
			m_isProjectionMatrixDirty = true;
		}
	}

	void ProjectorComponent::SetFieldOfView(const float fieldOfView)
	{
		if (m_fieldOfView != fieldOfView)
		{
			m_fieldOfView = fieldOfView;
			m_isProjectionMatrixDirty = true;
		}
	}

	void ProjectorComponent::SetNearPlaneDistance(const float nearPlaneDistance)
	{
		if (m_nearPlaneDistance != nearPlaneDistance)
		{
			m_nearPlaneDistance = nearPlaneDistance;
			m_isProjectionMatrixDirty = true;
		}
	}

	void ProjectorComponent::SetFarPlaneDistance(const float farPlaneDistance)
	{
		if (m_farPlaneDistance != farPlaneDistance)
		{
			m_farPlaneDistance = farPlaneDistance;
			m_isProjectionMatrixDirty = true;
		}
	}

	//-------------------------------------------------------------------------

	void ProjectorComponent::Reset()
	{
		// view
		{
			m_position = math::Vector3::Zero;

			m_direction = math::Vector3::Forward;
			m_up = math::Vector3::Up;
			m_right = math::Vector3::Right;

			m_viewMatrix = math::Matrix4::Identity;
			m_isViewMatrixDirty = true;
		}

		// projection
		{
			m_fieldOfView = k_defaultFieldOfView;
			m_aspectRatio = k_defaultAspectRatio;
			m_nearPlaneDistance = k_defaultNearPlaneDistance;
			m_farPlaneDistance = k_defaultFarPlaneDistance;

			m_projectionMatrix = math::Matrix4::Identity;
			m_isProjectionMatrixDirty = true;
		}

		m_viewProjectionMatrix = math::Matrix4::Identity;
	}

	//-------------------------------------------------------------------------

	void ProjectorComponent::Initialize(const Application& app)
	{
		Component::Initialize(app);

		// First time init matrices
		UpdateViewMatrix();
		UpdateProjectionMatrix();
		UpdateViewProjectionMatrix();
	}

	//-------------------------------------------------------------------------

	void ProjectorComponent::Update(const Time& time)
	{
		const bool isViewUpdated = UpdateViewMatrix();
		const bool isProjectionUpdated = UpdateProjectionMatrix();

		if (isViewUpdated || isProjectionUpdated)
			UpdateViewProjectionMatrix();
	}

	//-------------------------------------------------------------------------

	bool ProjectorComponent::UpdateViewMatrix()
	{
		if (!m_isViewMatrixDirty)
			return false;

		m_viewMatrix = math::Matrix4::LookToRH(m_position, m_direction, m_up);
		m_isViewMatrixDirty = false;

		return true;
	}

	bool ProjectorComponent::UpdateProjectionMatrix()
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

	void ProjectorComponent::UpdateViewProjectionMatrix()
	{
		m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
	}

	//-------------------------------------------------------------------------

	void ProjectorComponent::ApplyRotation(const math::Matrix4& transform)
	{
		const auto direction = m_direction.TransformNormal(transform);
		auto up = m_up.TransformNormal(transform);

		const auto right = direction.Cross(up);
		up = right.Cross(direction);

		if (m_direction != direction)
		{
			m_direction = direction;
			m_isViewMatrixDirty = true;
		}

		if (m_up != up)
		{
			m_up = up;
			m_isViewMatrixDirty = true;
		}

		if (m_right != right)
		{
			m_right = right;
			m_isViewMatrixDirty = true;
		}
	}
} // namespace library
