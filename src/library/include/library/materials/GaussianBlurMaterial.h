#pragma once
#include "library/materials/PostProcessingMaterial.h"

namespace library
{
	class GaussianBlurMaterial : public PostProcessingMaterial
	{
		RTTI_CLASS(GaussianBlurMaterial, PostProcessingMaterial)

	public:
		explicit GaussianBlurMaterial(const Effect& effect);
		~GaussianBlurMaterial();

		EffectVariable& GetSampleOffsets() const { return m_sampleOffsets; }
		EffectVariable& GetSampleWeights() const { return m_sampleWeights; }

	private:
		EffectVariable& m_sampleOffsets;
		EffectVariable& m_sampleWeights;
	};
} // namespace library
