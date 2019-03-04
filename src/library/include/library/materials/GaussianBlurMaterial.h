#pragma once
#include "library/Materials/PostProcessingMaterial.h"

namespace library
{
	class GaussianBlurMaterial : public PostProcessingMaterial
	{
		RTTI_CLASS(GaussianBlurMaterial, PostProcessingMaterial)

	public:
		explicit GaussianBlurMaterial(Effect& effect);
		~GaussianBlurMaterial();

		EffectVariable& GetSampleOffsets() { return m_sampleOffsets; }
		EffectVariable& GetSampleWeights() { return m_sampleWeights; }

	private:
		EffectVariable& m_sampleOffsets;
		EffectVariable& m_sampleWeights;
	};
} // namespace library
