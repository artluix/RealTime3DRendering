#include "StdAfx.h"
#include "library/components/SpotlightComponent.h"

namespace library
{
	namespace
	{
		constexpr float k_defaultInnerAngle = 0.75f;
		constexpr float k_defaultOuterAngle = 0.25f;
	}

	//-------------------------------------------------------------------------

	SpotlightComponent::SpotlightComponent()
		: LightComponent()
		, m_innerAngle(k_defaultInnerAngle)
		, m_outerAngle(k_defaultOuterAngle)
	{
	}

	SpotlightComponent::~SpotlightComponent() = default;

	//-------------------------------------------------------------------------

	void SpotlightComponent::SetInnerAngle(const float innerAngle)
	{
		m_innerAngle = innerAngle;
	}

	void SpotlightComponent::SetOuterAngle(const float outerAngle)
	{
		m_outerAngle = outerAngle;
	}
} // namespace library
