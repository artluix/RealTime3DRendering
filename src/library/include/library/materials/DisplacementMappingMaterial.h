#pragma once
#include "library/Materials/PointLightMaterial.h"

namespace library
{
class DisplacementMappingMaterial : public PointLightMaterial
{
	RTTI_CLASS(DisplacementMappingMaterial, PointLightMaterial)

public:
	explicit DisplacementMappingMaterial(std::shared_ptr<Effect> effect);
	~DisplacementMappingMaterial();

	EffectVariable& GetDisplacementScale() { return m_displacementScale; }
	EffectVariable& GetDisplacementMap() { return m_displacementMap; }

private:
	EffectVariable& m_displacementScale;
	EffectVariable& m_displacementMap;
};
} // namespace library
