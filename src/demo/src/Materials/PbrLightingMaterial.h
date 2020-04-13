#pragma once
#include "Materials/LightsMaterial.h"

class PbrLightingMaterial : public LightsMaterial
{
	RTTI_CLASS(PbrLightingMaterial, LightsMaterial)

public:
	library::EffectVariable& GetAmbientColor() { return m_ambientColor; }

protected:
	explicit PbrLightingMaterial(library::Effect& effect, const std::string& defaultTechniqueName);

private:
	library::EffectVariable& m_ambientColor;
};
