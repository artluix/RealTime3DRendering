#include "StdAfx.h"
#include "library/components/DirectionalLightComponent.h"

namespace library
{
	DirectionalLightComponent::DirectionalLightComponent(const Application& app)
		: Class(app)
		, m_direction(math::constants::Vector3::Forward)
		, m_up(math::constants::Vector3::Up)
		, m_right(math::constants::Vector3::Right)
	{
	}

	void DirectionalLightComponent::ApplyRotation(const math::Matrix4& transform)
	{
		const auto direction = m_direction.TransformNormal(transform);
		auto up = m_up.TransformNormal(transform);

		const auto right = direction.Cross(up);
		up = right.Cross(direction);

		m_direction = direction;
		m_up = up;
		m_right = right;
	}

} // namespace library


