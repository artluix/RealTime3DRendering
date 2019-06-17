#pragma once
#include "library/Materials/PostProcessingMaterial.h"

namespace library
{
class DistortionMappingMaterial : public PostProcessingMaterial
{
	RTTI_CLASS(DistortionMappingMaterial, PostProcessingMaterial)

public:
	explicit DistortionMappingMaterial(Effect& effect);
	~DistortionMappingMaterial();

	EffectVariable& GetWVP() { return m_wvp; }
	EffectVariable& GetDistortionMapTexture() { return m_distortionMapTexture; }
	EffectVariable& GetDisplacementScale() { return m_displacementScale; }

private:
	EffectVariable& m_wvp;
	EffectVariable& m_distortionMapTexture;
	EffectVariable& m_displacementScale;
};
} // namespace library
