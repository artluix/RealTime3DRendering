#include "Materials/SpotlightMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

SpotlightMaterial::SpotlightMaterial(Effect& effect)
	: PointLightMaterial(effect, "main11")

	, m_spotlightInnerAngle(effect.GetVariable("spotlightInnerAngle"))
	, m_spotlightOuterAngle(effect.GetVariable("spotlightOuterAngle"))
{}

SpotlightMaterial::~SpotlightMaterial() = default;
