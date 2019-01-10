#include "StdAfx.h"
#include "library/components/SpotLightComponent.h"

namespace library
{
	namespace components
	{
		namespace
		{
			constexpr float k_defaultInnerAngle = 0.75f;
			constexpr float k_defaultOuterAngle = 0.25f;
		}

		SpotLight::SpotLight()
			: Light()
			, m_innerAngle(k_defaultInnerAngle)
			, m_outerAngle(k_defaultOuterAngle)
		{
		}

		void SpotLight::SetInnerAngle(const float innerAngle)
		{
			m_innerAngle = innerAngle;
		}

		void SpotLight::SetOuterAngle(const float outerAngle)
		{
			m_outerAngle = outerAngle;
		}

	} // namespace components
} // namespace library
