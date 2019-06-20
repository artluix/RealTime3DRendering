#include "Materials/DisplacementMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

DisplacementMappingMaterial::DisplacementMappingMaterial(Effect& effect)
	: PointLightMaterial(effect, "main10")

	, m_displacementScale(effect.GetVariable("displacementScale"))
	, m_displacementMap(effect.GetVariable("DisplacementMap"))
{}

DisplacementMappingMaterial::~DisplacementMappingMaterial() = default;
