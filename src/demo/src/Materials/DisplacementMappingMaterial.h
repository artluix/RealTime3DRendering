#pragma once
#include "Materials/PhongLightingMaterial.h"

class DisplacementMappingMaterial : public PhongLightingMaterial
{
	RTTI_CLASS(DisplacementMappingMaterial, PhongLightingMaterial)

public:
	explicit DisplacementMappingMaterial(library::Effect& effect);
	~DisplacementMappingMaterial();

	library::EffectVariable& GetDisplacementScale() { return m_displacementScale; }
	library::EffectVariable& GetDisplacementMap() { return m_displacementMap; }

private:
	library::EffectVariable& m_displacementScale;
	library::EffectVariable& m_displacementMap;
};
