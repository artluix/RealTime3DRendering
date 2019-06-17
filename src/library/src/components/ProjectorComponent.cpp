#include "StdAfx.h"
#include "library/Components/ProjectorComponent.h"

#include "library/Math/Math.h"

namespace library
{

namespace
{
constexpr auto k_defaultNearPlaneDistance = 0.5f;
constexpr auto k_defaultFarPlaneDistance = 100.f;

constexpr auto k_defaultFieldOfView = math::Pi_Div_4;
constexpr auto k_defaultAspectRatio = 4.f / 3.f;

constexpr unsigned k_defaultWidth = 5;
constexpr unsigned k_defaultHeight = 5;
} // namespace

//-------------------------------------------------------------------------

ProjectorComponent::ProjectorComponent(const ProjectionType projectionType /*= ProjectionType::Perspective*/)
	: m_projectionType(projectionType)
	, m_nearPlaneDistance(k_defaultNearPlaneDistance)
	, m_farPlaneDistance(k_defaultFarPlaneDistance)
{
	switch (projectionType)
	{
		case ProjectionType::Perspective:
			m_fieldOfView = k_defaultFieldOfView;
			m_aspectRatio = k_defaultAspectRatio;
			break;

		case ProjectionType::Orthographic:
			m_width = k_defaultWidth;
			m_height = k_defaultHeight;
			break;

		default:
			break;
	}
}

ProjectorComponent::ProjectorComponent(
	const float nearPlaneDistance,
	const float farPlaneDistance,
	const float fieldOfView,
	const float aspectRatio)
	: m_projectionType(ProjectionType::Perspective)
	, m_nearPlaneDistance(nearPlaneDistance)
	, m_farPlaneDistance(farPlaneDistance)
	, m_fieldOfView(fieldOfView)
	, m_aspectRatio(aspectRatio)
{}

ProjectorComponent::ProjectorComponent(
	const float nearPlaneDistance,
	const float farPlaneDistance,
	const unsigned int width,
	const unsigned int height)
	: m_projectionType(ProjectionType::Orthographic)
	, m_nearPlaneDistance(nearPlaneDistance)
	, m_farPlaneDistance(farPlaneDistance)
	, m_width(width)
	, m_height(height)
{}

ProjectorComponent::~ProjectorComponent() = default;

//-------------------------------------------------------------------------

void ProjectorComponent::SetProjectionType(const ProjectionType projectionType)
{
	m_projectionType = projectionType;
}

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

//-------------------------------------------------------------------------

float ProjectorComponent::GetFieldOfView() const
{
	assert(m_projectionType == ProjectionType::Perspective);
	return m_fieldOfView;
}

void ProjectorComponent::SetFieldOfView(const float fieldOfView)
{
	assert(m_projectionType == ProjectionType::Perspective);
	if (m_fieldOfView != fieldOfView)
	{
		m_fieldOfView = fieldOfView;
		m_isProjectionMatrixDirty = true;
	}
}

float ProjectorComponent::GetAspectRatio() const
{
	assert(m_projectionType == ProjectionType::Perspective);
	return m_aspectRatio;
}

void ProjectorComponent::SetAspectRatio(const float aspectRatio)
{
	assert(m_projectionType == ProjectionType::Perspective);
	if (m_aspectRatio != aspectRatio)
	{
		m_aspectRatio = aspectRatio;
		m_isProjectionMatrixDirty = true;
	}
}

//-------------------------------------------------------------------------

unsigned int ProjectorComponent::GetWidth() const
{
	assert(m_projectionType == ProjectionType::Orthographic);
	return m_width;
}

void ProjectorComponent::SetWidth(const unsigned int width)
{
	assert(m_projectionType == ProjectionType::Orthographic);
	if (m_width != width)
	{
		m_width = width;
		m_isProjectionMatrixDirty = true;
	}
}

unsigned int ProjectorComponent::GetHeight() const
{
	assert(m_projectionType == ProjectionType::Orthographic);
	return m_height;
}

void ProjectorComponent::SetHeight(const unsigned int height)
{
	assert(m_projectionType == ProjectionType::Orthographic);
	if (m_height != height)
	{
		m_height = height;
		m_isProjectionMatrixDirty = true;
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

void ProjectorComponent::InitializeInternal()
{
	// First time init matrices
	UpdateViewMatrix();
	UpdateProjectionMatrix();
	UpdateViewProjectionMatrix();
}

//-------------------------------------------------------------------------

void ProjectorComponent::Update(const Time& time)
{
	SceneComponent::Update(time);

	const bool isViewProjectionMatrixDirty = m_isViewMatrixDirty || m_isProjectionMatrixDirty;

	UpdateViewMatrix();
	UpdateProjectionMatrix();

	if (isViewProjectionMatrixDirty)
		UpdateViewProjectionMatrix();
}

//-------------------------------------------------------------------------

void ProjectorComponent::UpdateViewMatrix()
{
	if (!m_isViewMatrixDirty)
		return;

	m_viewMatrix = math::Matrix4::LookToRH(m_transform.position, m_direction, m_up);
	m_isViewMatrixDirty = false;
}

void ProjectorComponent::UpdateProjectionMatrix()
{
	if (!m_isProjectionMatrixDirty)
		return;

	switch (m_projectionType)
	{
		case ProjectionType::Orthographic:
			m_projectionMatrix = math::Matrix4::OrthographicRH(
				static_cast<float>(m_width),
				static_cast<float>(m_height),
				m_nearPlaneDistance,
				m_farPlaneDistance
			);
			break;

		case ProjectionType::Perspective:
			m_projectionMatrix = math::Matrix4::PerspectiveFovRH(
				m_fieldOfView,
				m_aspectRatio,
				m_nearPlaneDistance,
				m_farPlaneDistance
			);
			break;

		default: break;
	}

	m_isProjectionMatrixDirty = false;
}

void ProjectorComponent::UpdateViewProjectionMatrix()
{
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

//-------------------------------------------------------------------------

void ProjectorComponent::OnTranslation(const math::Vector3& translationDelta)
{
	m_isViewMatrixDirty = true;
}

void ProjectorComponent::OnRotation(const math::Quaternion& rotationDelta)
{
	m_isViewMatrixDirty = true;
}
} // namespace library
