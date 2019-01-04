#pragma once
#include "library/lights/DirectionalLight.h"
#include "library/lights/PointLight.h"

namespace library
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
} // namespace library

