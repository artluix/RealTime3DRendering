#include "StdAfx.h"
#include "library/components/DirectionalLightComponent.h"

namespace library
{
	namespace components
	{
		DirectionalLight::DirectionalLight()
			: Light()
			, m_direction(math::constants::Vector3::Forward)
			, m_up(math::constants::Vector3::Up)
			, m_right(math::constants::Vector3::Right)
		{
		}

		void DirectionalLight::ApplyRotation(const math::Matrix4& transform)
		{
			const auto direction = m_direction.TransformNormal(transform);
			auto up = m_up.TransformNormal(transform);

			const auto right = direction.Cross(up);
			up = right.Cross(direction);

			m_direction = direction;
			m_up = up;
			m_right = right;
		}

	} // namespace components
} // namespace library
