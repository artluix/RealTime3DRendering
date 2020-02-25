#pragma once
#include "Materials/LightMaterial.h"

class DisplacementMappingMaterial : public LightMaterial
{
	RTTI_CLASS(DisplacementMappingMaterial, LightMaterial)

public:
	explicit DisplacementMappingMaterial(library::Effect& effect);
	~DisplacementMappingMaterial();

	library::EffectVariable& GetDisplacementScale() { return m_displacementScale; }
	library::EffectVariable& GetDisplacementMap() { return m_displacementMap; }

private:
	library::EffectVariable& m_displacementScale;
	library::EffectVariable& m_displacementMap;
};
