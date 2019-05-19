#pragma once
#include "library/Materials/LightMaterial.h"

namespace library
{
class PointLightMaterial : public LightMaterial
{
	RTTI_CLASS(PointLightMaterial, LightMaterial)

public:
	explicit PointLightMaterial(std::shared_ptr<Effect> effect, const std::string& defaultTechniqueName = "main11");
	~PointLightMaterial();

	EffectVariable& GetLightPosition() { return m_lightPosition; }
	EffectVariable& GetLightRadius() { return m_lightRadius; }

private:
	EffectVariable& m_lightPosition;
	EffectVariable& m_lightRadius;
};
} // namespace library
