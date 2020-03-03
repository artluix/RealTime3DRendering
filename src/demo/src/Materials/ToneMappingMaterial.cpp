#include "Materials/ToneMappingMaterial.h"

#include <library/Effect/Effect.h>

ToneMappingMaterial::ToneMappingMaterial(library::Effect& effect)
	: PostProcessingMaterial(effect)
	, m_exposure(effect.GetVariable("Exposure"))
{
}

ToneMappingMaterial::~ToneMappingMaterial() = default;
