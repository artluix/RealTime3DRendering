#include "StdAfx.h"
#include "library/Components/DirectionalLightComponent.h"

namespace library
{
	DirectionalLightComponent::DirectionalLightComponent()
		: m_direction(math::Vector3::Forward)
		, m_up(math::Vector3::Up)
		, m_right(math::Vector3::Right)
	{
	}

	DirectionalLightComponent::~DirectionalLightComponent() = default;

	//-------------------------------------------------------------------------

	void DirectionalLightComponent::Rotate(const math::Vector3& rotation)
	{
		if (!rotation)
			return;

		m_rotation += rotation;
		SetRotation(m_rotation);
	}

	void DirectionalLightComponent::SetRotation(const math::Vector3& rotation)
	{
		const auto rotationMatrix = math::Matrix4::RotationPitchYawRoll(rotation);

		m_direction = rotationMatrix.GetForward();
		m_right = rotationMatrix.GetRight();
		m_up = rotationMatrix.GetUp();
	}
} // namespace library
