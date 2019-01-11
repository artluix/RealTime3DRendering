#include "StdAfx.h"
#include "library/components/PointLightComponent.h"

namespace library
{
	namespace
	{
		constexpr float k_defaultRadius = 10.f;
	}

	PointLightComponent::PointLightComponent()
		: LightComponent()
		, m_position(math::Vector3::Zero)
		, m_radius(k_defaultRadius)
	{
	}

	void PointLightComponent::SetPosition(const math::Vector3& position)
	{
		if (m_position != position)
		{
			m_position = position;
		}
	}

	void PointLightComponent::SetRadius(const float radius)
	{
		m_radius = radius;
	}
} // namespace library
