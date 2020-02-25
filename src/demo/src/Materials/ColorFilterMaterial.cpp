#include "Materials/ColorFilterMaterial.h"

#include <library/Effect/Effect.h>

ColorFilterMaterial::ColorFilterMaterial(library::Effect& effect)
	: PostProcessingMaterial(effect)
	, m_colorFilter(effect.GetVariable("ColorFilter"))
{}

ColorFilterMaterial::~ColorFilterMaterial() = default;
