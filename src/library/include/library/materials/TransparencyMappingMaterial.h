#pragma once
#include "library/Materials/PointLightMaterial.h"

namespace library
{
class TransparencyMappingMaterial : public PointLightMaterial
{
	RTTI_CLASS(TransparencyMappingMaterial, PointLightMaterial)

public:
	explicit TransparencyMappingMaterial(Effect& effect);
	~TransparencyMappingMaterial();

	EffectVariable& GetFogColor() { return m_fogColor; }
	EffectVariable& GetFogStart() { return m_fogStart; }
	EffectVariable& GetFogRange() { return m_fogRange; }

	EffectVariable& GetTransparencyMap() { return m_transparencyMap; }

protected:
	void InitializeInternal() override;

private:
	EffectVariable& m_fogColor;
	EffectVariable& m_fogStart;
	EffectVariable& m_fogRange;

	EffectVariable& m_transparencyMap;
};
} // namespace library
