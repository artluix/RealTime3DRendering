#include "StdAfx.h"
#include "library/Materials/ToneMappingMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
ToneMappingMaterial::ToneMappingMaterial(Effect& effect)
	: PostProcessingMaterial(effect)
	, m_exposure(effect.GetVariable("Exposure"))
{
}

ToneMappingMaterial::~ToneMappingMaterial() = default;
} // namespace library
