#pragma once
#include "Materials/LightMaterial.h"

class FogMaterial : public LightMaterial
{
	RTTI_CLASS(FogMaterial, LightMaterial)

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
