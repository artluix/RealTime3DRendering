#include "StdAfx.h"
#include "library/components/SpotLightComponent.h"

namespace library
{
	namespace
	{
		constexpr float k_defaultInnerAngle = 0.75f;
		constexpr float k_defaultOuterAngle = 0.25f;
	}

	SpotLightComponent::SpotLightComponent()
		: LightComponent()
		, m_innerAngle(k_defaultInnerAngle)
		, m_outerAngle(k_defaultOuterAngle)
	{
	}

	void SpotLightComponent::SetInnerAngle(const float innerAngle)
	{
		m_innerAngle = innerAngle;
	}

	void SpotLightComponent::SetOuterAngle(const float outerAngle)
	{
		m_outerAngle = outerAngle;
	}
} // namespace library
