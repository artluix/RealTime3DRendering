#pragma once
#include "library/components/DirectionalLightComponent.h"
#include "library/components/PointLightComponent.h"

namespace library
{
	namespace components
	{
		class SpotLight
			: public DirectionalLight
			, public PointLight
		{
		public:
			explicit SpotLight();

			float GetInnerAngle() const { return m_innerAngle; }
			void SetInnerAngle(const float innerAngle);

			float GetOuterAngle() const { return m_outerAngle; }
			void SetOuterAngle(const float outerAngle);

		protected:
			float m_innerAngle, m_outerAngle;
		};

	} // namespace components
} // namespace library
