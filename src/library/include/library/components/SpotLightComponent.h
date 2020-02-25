#pragma once
#include "library/Components/DirectionalLightComponent.h"
#include "library/Components/PointLightComponent.h"

namespace library
{
__declspec(align(16)) struct SpotlightData
{
	math::Color color;
	math::Vector3 direction;
	float radius;
	math::Vector3 position;
	float innerAngle, outerAngle;
};

//-------------------------------------------------------------------------

class SpotlightComponent
	: public DirectionalLightComponent
	, public PointLightComponent
{
	RTTI_CLASS(SpotlightComponent, DirectionalLightComponent, PointLightComponent)

public:
	using Data = SpotlightData;

	SpotlightComponent();
	~SpotlightComponent();

	Data GetData() const;

	float GetInnerAngle() const { return m_innerAngle; }
	void SetInnerAngle(const float innerAngle);

	float GetOuterAngle() const { return m_outerAngle; }
	void SetOuterAngle(const float outerAngle);

protected:
	float m_innerAngle, m_outerAngle;
};
} // namespace library
