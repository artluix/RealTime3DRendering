#pragma once
#include "Materials/PointLightMaterial.h"

class SpotlightMaterial : public PointLightMaterial
{
	RTTI_CLASS(SpotlightMaterial, PointLightMaterial)

public:
	explicit SpotlightMaterial(library::Effect& effect);
	~SpotlightMaterial();

	library::EffectVariable& GetSpotlightInnerAngle() { return m_spotlightInnerAngle; }
	library::EffectVariable& GetSpotlightOuterAngle() { return m_spotlightOuterAngle; }

private:
	library::EffectVariable& m_spotlightInnerAngle;
	library::EffectVariable& m_spotlightOuterAngle;
};
