#pragma once
#include "Materials/PhongLightingMaterial.h"

class FogMaterial : public PhongLightingMaterial
{
	RTTI_CLASS(FogMaterial, PhongLightingMaterial)

public:
	explicit FogMaterial(library::Effect& effect);
	~FogMaterial();

	library::EffectVariable& GetFogColor() { return m_fogColor; }
	library::EffectVariable& GetFogStart() { return m_fogStart; }
	library::EffectVariable& GetFogRange() { return m_fogRange; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_fogColor;
	library::EffectVariable& m_fogStart;
	library::EffectVariable& m_fogRange;
};
