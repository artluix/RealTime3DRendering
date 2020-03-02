#include "Materials/ToneMappingMaterial.h"

#include <library/Effect/Effect.h>

ToneMappingMaterial::ToneMappingMaterial(library::Effect& effect)
	: PostProcessingMaterial(effect)
{
}

ToneMappingMaterial::~ToneMappingMaterial() = default;
