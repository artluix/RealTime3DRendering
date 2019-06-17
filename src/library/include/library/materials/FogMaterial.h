#pragma once
#include "library/Materials/DirectionalLightMaterial.h"

namespace library
{
class FogMaterial : public DirectionalLightMaterial
{
	RTTI_CLASS(FogMaterial, DirectionalLightMaterial)

public:
	explicit FogMaterial(Effect& effect);
	~FogMaterial();

	EffectVariable& GetFogColor() { return m_fogColor; }
	EffectVariable& GetFogStart() { return m_fogStart; }
	EffectVariable& GetFogRange() { return m_fogRange; }

protected:
	void InitializeInternal() override;

private:
	EffectVariable& m_fogColor;
	EffectVariable& m_fogStart;
	EffectVariable& m_fogRange;
};
} // namespace library
