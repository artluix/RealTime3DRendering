#include "Materials/DirectionalLightMaterial.h"

#include <library/Effect/Effect.h>

DirectionalLightMaterial::DirectionalLightMaterial(
	library::Effect& effect,
	const std::string& defaultTechniqueName /*= "main11"*/
)
	: LightMaterial(effect, defaultTechniqueName)

	, m_lightDirection(effect.GetVariable("lightDirection"))
{}

DirectionalLightMaterial::~DirectionalLightMaterial() = default;
