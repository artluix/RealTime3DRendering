#pragma once
#include "library/materials/PostProcessingMaterial.h"

namespace library
{
	class DistortionMappingMaterial : public PostProcessingMaterial
	{
		RTTI_CLASS(DistortionMappingMaterial, PostProcessingMaterial)

	public:
		explicit DistortionMappingMaterial(const Effect& effect);
		~DistortionMappingMaterial();

		EffectVariable& GetWVP() const { return m_wvp; }
		EffectVariable& GetDistortionMapTexture() const { return m_distortionMapTexture; }
		EffectVariable& GetDisplacementScale() const { return m_displacementScale; }

	private:
		EffectVariable& m_wvp;
		EffectVariable& m_distortionMapTexture;
		EffectVariable& m_displacementScale;
	};
} // namespace library
