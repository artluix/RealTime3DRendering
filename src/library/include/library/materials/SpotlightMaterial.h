#pragma once
#include "library/Materials/PointLightMaterial.h"

namespace library
{
class SpotlightMaterial : public PointLightMaterial
{
	RTTI_CLASS(SpotlightMaterial, PointLightMaterial)

public:
	explicit SpotlightMaterial(Effect& effect);
	~SpotlightMaterial();

	EffectVariable& GetLightLookAt() { return m_lightLookAt; }
	EffectVariable& GetSpotlightInnerAngle() { return m_spotlightInnerAngle; }
	EffectVariable& GetSpotlightOuterAngle() { return m_spotlightOuterAngle; }

private:
	EffectVariable& m_lightLookAt;
	EffectVariable& m_spotlightInnerAngle;
	EffectVariable& m_spotlightOuterAngle;
};
} // namespace library
