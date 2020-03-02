#pragma once
#include "Materials/LightsMaterial.h"

class DisplacementMappingMaterial : public LightsMaterial
{
	RTTI_CLASS(DisplacementMappingMaterial, LightsMaterial)

public:
	explicit DisplacementMappingMaterial(library::Effect& effect);
	~DisplacementMappingMaterial();

	library::EffectVariable& GetDisplacementScale() { return m_displacementScale; }
	library::EffectVariable& GetDisplacementMap() { return m_displacementMap; }

private:
	library::EffectVariable& m_displacementScale;
	library::EffectVariable& m_displacementMap;
};
