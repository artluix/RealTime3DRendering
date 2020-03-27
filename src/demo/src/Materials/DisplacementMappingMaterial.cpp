#include "Materials/DisplacementMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

DisplacementMappingMaterial::DisplacementMappingMaterial(Effect& effect)
	: PhongLightingMaterial(effect, "main11")

	, m_displacementScale(effect.GetVariable("DisplacementScale"))
	, m_displacementMap(effect.GetVariable("DisplacementMap"))
{}

DisplacementMappingMaterial::~DisplacementMappingMaterial() = default;
