#pragma once
#include "library/Materials/PostProcessingMaterial.h"

namespace library
{
class ToneMappingMaterial : public PostProcessingMaterial
{
	RTTI_CLASS(ToneMappingMaterial, PostProcessingMaterial)

public:
	explicit ToneMappingMaterial(Effect& effect);
	~ToneMappingMaterial();

	EffectVariable& GetLuminanceLogTexture() { return m_luminanceLogTexture; }
	EffectVariable& GetLuminanceSamplingStepsCount() { return m_luminanceSamplingStepsCount; }
	EffectVariable& GetLuminanceSamplingMipsCount() { return m_luminanceSamplingMipsCount; }

private:
	EffectVariable& m_luminanceLogTexture;
	EffectVariable& m_luminanceSamplingStepsCount;
	EffectVariable& m_luminanceSamplingMipsCount;
};
} // namespace library
