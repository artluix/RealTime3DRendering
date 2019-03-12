#include "StdAfx.h"
#include "library/Components/ProjectorComponent.h"

namespace library
{
	namespace
	{
		constexpr auto k_defaultNearPlaneDistance = 0.5f;
		constexpr auto k_defaultFarPlaneDistance = 100.f;
	}

	//-------------------------------------------------------------------------

	ProjectorComponent::ProjectorComponent()
		: ProjectorComponent(
			k_defaultNearPlaneDistance,
			k_defaultFarPlaneDistance
		)
	{
	}

	ProjectorComponent::ProjectorComponent(
		const float nearPlaneDistance,
		const float farPlaneDistance
	)
		: m_position(math::Vector3::Zero)

		, m_nearPlaneDistance(nearPlaneDistance)
		, m_farPlaneDistance(farPlaneDistance)

		, m_direction(math::Vector3::Forward)
		, m_up(math::Vector3::Up)
		, m_right(math::Vector3::Right)

		, m_viewMatrix(math::Matrix4::Identity)
		, m_projectionMatrix(math::Matrix4::Identity)
		, m_viewProjectionMatrix(math::Matrix4::Identity)
	{
	}

	ProjectorComponent::~ProjectorComponent() = default;

	//-------------------------------------------------------------------------

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

	void ProjectorComponent::SetPosition(const math::Vector3& position)
	{
		if (m_position != position)
		{
			m_position = position;
			m_isViewMatrixDirty = true;
		}
	}

	//-------------------------------------------------------------------------

	const math::Matrix4& ProjectorComponent::GetViewMatrix() const
	{
		assert(!m_isViewMatrixDirty);
		return m_viewMatrix;
	}

	const math::Matrix4& ProjectorComponent::GetProjectionMatrix() const
	{
		assert(!m_isProjectionMatrixDirty);
		return m_projectionMatrix;
	}

	const math::Matrix4& ProjectorComponent::GetViewProjectionMatrix() const
	{
		assert(!m_isViewMatrixDirty && !m_isProjectionMatrixDirty);
		return m_viewProjectionMatrix;
	}
	//-------------------------------------------------------------------------

	void ProjectorComponent::Reset()
	{
		// projection
		{
			m_nearPlaneDistance = k_defaultNearPlaneDistance;
			m_farPlaneDistance = k_defaultFarPlaneDistance;

			m_projectionMatrix = math::Matrix4::Identity;
			m_isProjectionMatrixDirty = true;
		}

		// view
		{
			m_position = math::Vector3::Zero;

			m_direction = math::Vector3::Forward;
			m_up = math::Vector3::Up;
			m_right = math::Vector3::Right;

			m_viewMatrix = math::Matrix4::Identity;
			m_isViewMatrixDirty = true;
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
