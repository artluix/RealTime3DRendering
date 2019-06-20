#pragma once
#include "library/Materials/PostProcessingMaterial.h"

class DistortionMappingMaterial : public library::PostProcessingMaterial
{
	RTTI_CLASS(DistortionMappingMaterial, library::PostProcessingMaterial)

public:
	explicit DistortionMappingMaterial(library::Effect& effect);
	~DistortionMappingMaterial();

	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetDistortionMapTexture() { return m_distortionMapTexture; }
	library::EffectVariable& GetDisplacementScale() { return m_displacementScale; }

private:
	library::EffectVariable& m_wvp;
	library::EffectVariable& m_distortionMapTexture;
	library::EffectVariable& m_displacementScale;
};
