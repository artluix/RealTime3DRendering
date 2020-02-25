#include "StdAfx.h"
#include "library/Components/DirectionalLightComponent.h"

#include "library/Math/Direction.h"
#include "library/Math/Matrix.h"

namespace library
{

namespace
{
constexpr auto k_defaultDirection = math::Direction::Forward;
}

DirectionalLightComponent::DirectionalLightComponent()
	: m_direction(k_defaultDirection)
{}

DirectionalLightComponent::~DirectionalLightComponent() = default;

//-------------------------------------------------------------------------

DirectionalLightComponent::Data DirectionalLightComponent::GetData() const
{
	return Data{ m_color, m_direction };
}

void DirectionalLightComponent::SetDirection(const math::Vector3& direction)
{
	m_direction = direction;
}

//-------------------------------------------------------------------------

void DirectionalLightComponent::Rotate(const math::Quaternion& rotation)
{
	if (rotation == math::Quaternion::Identity)
		return;

	const auto rotationMatrix = math::Matrix4::RotationQuaternion(rotation);
	m_direction = m_direction.Transform(rotationMatrix);
}

void DirectionalLightComponent::SetRotation(const math::Quaternion& rotation)
{
	const auto rotationMatrix = math::Matrix4::RotationQuaternion(rotation);
	m_direction = rotationMatrix.GetForward();
}
} // namespace library
