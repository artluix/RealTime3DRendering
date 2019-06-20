#pragma once
#include "Materials/DirectionalLightMaterial.h"

class FogMaterial : public DirectionalLightMaterial
{
	RTTI_CLASS(FogMaterial, DirectionalLightMaterial)

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
