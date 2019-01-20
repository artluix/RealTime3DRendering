#pragma once
#include "library/materials/PostProcessingMaterial.h"

namespace library
{
	struct GaussianBlurSample
	{
		struct
		{
			float x, y;
		} offset;

		float weight;
	};

	//-------------------------------------------------------------------------

	class GaussianBlurMaterial : public PostProcessingMaterial
	{
		RTTI_CLASS(GaussianBlurMaterial, PostProcessingMaterial)

	public:
		explicit GaussianBlurMaterial(const Effect& effect);
		~GaussianBlurMaterial();

		EffectVariable& GetSamples() const { return m_samples; }

	private:
		EffectVariable& m_samples;
	};
} // namespace library
