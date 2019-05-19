#include "StdAfx.h"
#include "library/Materials/SpotlightMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
SpotlightMaterial::SpotlightMaterial(std::shared_ptr<Effect> effect)
	: PointLightMaterial(effect, "main10")
	, m_lightLookAt(effect->GetVariable("lightLookAt"))
	, m_spotlightInnerAngle(effect->GetVariable("spotLightInnerAngle"))
	, m_spotlightOuterAngle(effect->GetVariable("spotLightOuterAngle"))
{}

SpotlightMaterial::~SpotlightMaterial() = default;
} // namespace library
