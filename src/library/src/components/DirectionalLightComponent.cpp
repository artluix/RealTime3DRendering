#include "StdAfx.h"
#include "library/components/DirectionalLightComponent.h"

namespace library
{
	DirectionalLightComponent::DirectionalLightComponent()
		: LightComponent()
		, m_direction(math::Vector3::Forward)
		, m_up(math::Vector3::Up)
		, m_right(math::Vector3::Right)
	{
	}

	DirectionalLightComponent::~DirectionalLightComponent() = default;

	void DirectionalLightComponent::Rotate(const math::Vector3& rotation)
	{
		if (!rotation)
			return;

		m_rotation += rotation;
		SetRotation(m_rotation);
	}

	void DirectionalLightComponent::SetRotation(const math::Vector3& rotation)
	{
		const auto rotationMatrix = math::Matrix4::RotationPitchYawRoll(m_rotation);

		m_direction = rotationMatrix.GetForward();
		m_up = rotationMatrix.GetUp();
		m_right = rotationMatrix.GetRight();
	}
} // namespace library
