#pragma once
#include "Materials/LightMaterial.h"

class PointLightMaterial : public LightMaterial
{
	RTTI_CLASS(PointLightMaterial, LightMaterial)

public:
	explicit PointLightMaterial(library::Effect& effect, const std::string& defaultTechniqueName = "main11");
	~PointLightMaterial();

	library::EffectVariable& GetLightData() { return m_lightData; }

	bool IsDeferredSupported() const { return m_isDeferredSupported; }

protected:
	bool m_isDeferredSupported = false;

private:
	library::EffectVariable& m_lightData;
};
