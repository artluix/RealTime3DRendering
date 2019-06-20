#pragma once
#include "Materials/LightMaterial.h"

class PointLightMaterial : public LightMaterial
{
	RTTI_CLASS(PointLightMaterial, LightMaterial)

public:
	explicit PointLightMaterial(library::Effect& effect, const std::string& defaultTechniqueName = "main11");
	~PointLightMaterial();

	library::EffectVariable& GetLightPosition() { return m_lightPosition; }
	library::EffectVariable& GetLightRadius() { return m_lightRadius; }

private:
	library::EffectVariable& m_lightPosition;
	library::EffectVariable& m_lightRadius;
};
