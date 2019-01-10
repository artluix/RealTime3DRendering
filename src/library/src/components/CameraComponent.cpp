#include "StdAfx.h"
#include "library/components/CameraComponent.h"

#include "library/Application.h"
#include "library/util/Utils.h"

namespace library
{
	namespace
	{
		constexpr auto k_defaultFieldOfView = math::constants::Pi_Div_4;
		constexpr auto k_defaultNearPlaneDistance = 0.01f;
		constexpr auto k_defaultFarPlaneDistance = 1000.f;
		const auto k_defaultFontPath = util::GetExecutableDirectory().Join(
			fs::Path("data/fonts/Arial_14_Regular.spritefont")
		);
	}

	Camera::Camera(const Application& app)
		: Component(app)
		, m_fieldOfView(k_defaultFieldOfView)
		, m_aspectRatio(app.GetAspectRatio())
		, m_nearPlaneDistance(k_defaultNearPlaneDistance)
		, m_farPlaneDistance(k_defaultFarPlaneDistance)
		, m_isViewMatrixDirty(true)
	{
	}

	Camera::Camera(
		const Application& app,
		const float fieldOfView,
		const float aspectRatio,
		const float nearPlaneDistance,
		const float farPlaneDistance
	)
		: Component(app)
		, m_fieldOfView(fieldOfView)
		, m_nearPlaneDistance(nearPlaneDistance)
		, m_farPlaneDistance(farPlaneDistance)
	{
	}

	Camera::~Camera()
	{
	}

	//-------------------------------------------------------------------------

	math::Matrix4 Camera::GetViewProjectionMatrix() const
	{
		return m_viewMatrix * m_projectionMatrix;
	}

	//-------------------------------------------------------------------------

	void Camera::SetPosition(const math::Vector3& position)
	{
		if (m_position != position)
		{
			m_position = position;
			m_isViewMatrixDirty = true;
		}
	}

	//-------------------------------------------------------------------------

	void Camera::Reset()
	{
		m_position = math::constants::Vector3::Zero;
		m_direction = math::constants::Vector3::Forward;
		m_up = math::constants::Vector3::Up;
		m_right = math::constants::Vector3::Right;

		UpdateViewMatrix();
	}

	//-------------------------------------------------------------------------

	void Camera::Initialize()
	{
		UpdateProjectionMatrix();
		Reset();
	}

	void Camera::Update(const Time& time)
	{
		if (m_isViewMatrixDirty)
		{
			UpdateViewMatrix();
			m_isViewMatrixDirty = false;
		}
	}

	//-------------------------------------------------------------------------

	void Camera::UpdateViewMatrix()
	{
		m_viewMatrix = math::Matrix4::LookToRH(m_position, m_direction, m_up);
	}

	void Camera::UpdateProjectionMatrix()
	{
		m_projectionMatrix = math::Matrix4::PerspectiveFovRH(
			m_fieldOfView,
			m_aspectRatio,
			m_nearPlaneDistance,
			m_farPlaneDistance
		);
	}

	//-------------------------------------------------------------------------

	void Camera::ApplyRotation(const math::Matrix4& transform)
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
