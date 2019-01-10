#include "StdAfx.h"
#include "library/components/PointLightComponent.h"

namespace library
{
	namespace components
	{
		namespace
		{
			constexpr float k_defaultRadius = 10.f;
		}

		PointLight::PointLight()
			: Light()
			, m_position(math::constants::Vector3::Zero)
			, m_radius(k_defaultRadius)
		{
		}

		void PointLight::SetPosition(const math::Vector3& position)
		{
			if (m_position != position)
			{
				m_position = position;
			}
		}

		void PointLight::SetRadius(const float radius)
		{
			m_radius = radius;
		}

	} // namespace components
} // namespace library
