#pragma once
#include "Materials/PointLightMaterial.h"

class DisplacementMappingMaterial : public PointLightMaterial
{
	RTTI_CLASS(DisplacementMappingMaterial, PointLightMaterial)

public:
	explicit DisplacementMappingMaterial(library::Effect& effect);
	~DisplacementMappingMaterial();

	library::EffectVariable& GetDisplacementScale() { return m_displacementScale; }
	library::EffectVariable& GetDisplacementMap() { return m_displacementMap; }

private:
	library::EffectVariable& m_displacementScale;
	library::EffectVariable& m_displacementMap;
};
