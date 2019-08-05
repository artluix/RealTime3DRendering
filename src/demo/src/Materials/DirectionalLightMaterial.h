#pragma once
#include "Materials/LightMaterial.h"

class DirectionalLightMaterial : public LightMaterial
{
	RTTI_CLASS(DirectionalLightMaterial, LightMaterial)

public:
	explicit DirectionalLightMaterial(library::Effect& effect, const std::string& defaultTechniqueName = "main11");
	~DirectionalLightMaterial();

	library::EffectVariable& GetLightData() { return m_lightData; }

private:
	library::EffectVariable& m_lightData;
};
