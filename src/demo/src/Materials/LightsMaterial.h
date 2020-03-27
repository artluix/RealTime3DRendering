#pragma once
#include "LightsData.h"

#include <library/Materials/Material.h>

class LightsMaterial : public library::Material
{
	RTTI_CLASS(LightsMaterial, library::Material)

public:
	library::EffectVariable& GetDirLights() { return m_dirLights; }
	library::EffectVariable& GetDirLightsCount() { return m_dirLightsCount; }

	library::EffectVariable& GetPointLights() { return m_pointLights; }
	library::EffectVariable& GetPointLightsCount() { return m_pointLightsCount; }

	library::EffectVariable& GetSpotlights() { return m_spotlights; }
	library::EffectVariable& GetSpotlightsCount() { return m_spotlightsCount; }

protected:
	explicit LightsMaterial(library::Effect& effect, const std::string& defaultTechniqueName);

private:
	library::EffectVariable& m_dirLights;
	library::EffectVariable& m_dirLightsCount;

	library::EffectVariable& m_pointLights;
	library::EffectVariable& m_pointLightsCount;

	library::EffectVariable& m_spotlights;
	library::EffectVariable& m_spotlightsCount;
};
