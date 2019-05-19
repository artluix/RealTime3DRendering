#pragma once
#include "library/Components/PointLightComponent.h"
#include "library/Components/DirectionalLightComponent.h"

namespace library
{
class SpotlightComponent
	: public DirectionalLightComponent
	, public PointLightComponent
{
public:
	SpotlightComponent();
	~SpotlightComponent();

	float GetInnerAngle() const { return m_innerAngle; }
	void SetInnerAngle(const float innerAngle);

	float GetOuterAngle() const { return m_outerAngle; }
	void SetOuterAngle(const float outerAngle);

protected:
	float m_innerAngle, m_outerAngle;
};
} // namespace library
