#pragma once
#include "Materials/LightMaterial.h"

class DirectionalLightMaterial : public LightMaterial
{
	RTTI_CLASS(DirectionalLightMaterial, LightMaterial)

public:
	explicit DirectionalLightMaterial(library::Effect& effect, const std::string& defaultTechniqueName = "main11");
	~DirectionalLightMaterial();

	library::EffectVariable& GetLightDirection() { return m_lightDirection; }

private:
	library::EffectVariable& m_lightDirection;
};
