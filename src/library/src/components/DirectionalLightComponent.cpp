#include "StdAfx.h"
#include "library/Components/DirectionalLightComponent.h"

#include "library/Math/Direction.h"
#include "library/Math/Matrix.h"

namespace library
{
DirectionalLightComponent::DirectionalLightComponent()
	: m_direction(math::Direction::Forward)
	, m_up(math::Direction::Up)
	, m_right(math::Direction::Right)

	, m_rotation(math::Quaternion::Identity)
{}

DirectionalLightComponent::~DirectionalLightComponent() = default;

//-------------------------------------------------------------------------

void DirectionalLightComponent::Rotate(const math::Quaternion& rotation)
{
	if (rotation == math::Quaternion::Identity)
		return;

	SetRotation(m_rotation * rotation);
}

void DirectionalLightComponent::SetRotation(const math::Quaternion& rotation)
{
	if (m_rotation == rotation)
		return;

	m_rotation = rotation;

	const auto rotationMatrix = math::Matrix4::RotationQuaternion(m_rotation);

	m_direction = rotationMatrix.GetForward();
	m_right = rotationMatrix.GetRight();
	m_up = rotationMatrix.GetUp();
}
} // namespace library
