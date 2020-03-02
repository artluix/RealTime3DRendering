#pragma once
#include "library/Components/DirectionalLightComponent.h"
#include "library/Components/PointLightComponent.h"

namespace library
{
class SpotlightComponent
	: public DirectionalLightComponent
	, public PointLightComponent
{
	RTTI_CLASS(SpotlightComponent, DirectionalLightComponent, PointLightComponent)

public:
	SpotlightComponent();
	~SpotlightComponent();

	float GetInnerAngle() const { return m_innerAngle; }
	void SetInnerAngle(const float innerAngle);

	float GetOuterAngle() const { return m_outerAngle; }
	void SetOuterAngle(const float outerAngle);

#ifdef USE_LIGHT_PROXY_MODEL
	void SetupProxyModel(const CameraComponent& camera) override;
#endif

protected:
	float m_innerAngle, m_outerAngle;
};
} // namespace library
