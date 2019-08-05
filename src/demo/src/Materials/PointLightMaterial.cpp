#include "Materials/PointLightMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

PointLightMaterial::PointLightMaterial(
	Effect& effect,
	const std::string& defaultTechniqueName /*= "main11"*/
)
	: LightMaterial(effect, defaultTechniqueName)

	, m_lightData(effect.GetVariable("lightData"))
{}

PointLightMaterial::~PointLightMaterial() = default;
