#pragma once
#include "Materials/LightMaterial.h"

class TransparencyMappingMaterial : public LightMaterial
{
	RTTI_CLASS(TransparencyMappingMaterial, LightMaterial)

public:
	explicit TransparencyMappingMaterial(library::Effect& effect);
	~TransparencyMappingMaterial();

	library::EffectVariable& GetFogColor() { return m_fogColor; }
	library::EffectVariable& GetFogStart() { return m_fogStart; }
	library::EffectVariable& GetFogRange() { return m_fogRange; }

	library::EffectVariable& GetTransparencyMap() { return m_transparencyMap; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_fogColor;
	library::EffectVariable& m_fogStart;
	library::EffectVariable& m_fogRange;

	library::EffectVariable& m_transparencyMap;
};
