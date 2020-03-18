#include "StdAfx.h"
#include "library/Materials/ToneMappingMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
ToneMappingMaterial::ToneMappingMaterial(Effect& effect)
	: PostProcessingMaterial(effect)
	, m_luminanceLogTexture(effect.GetVariable("LuminanceLogTexture"))
	, m_luminanceSamplingStepsCount(effect.GetVariable("LuminanceSamplingStepsCount"))
	, m_luminanceSamplingMipsCount(effect.GetVariable("LuminanceSamplingMipsCount"))
{
}

ToneMappingMaterial::~ToneMappingMaterial() = default;
} // namespace library
