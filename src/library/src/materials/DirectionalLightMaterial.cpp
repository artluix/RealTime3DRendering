#include "StdAfx.h"
#include "library/Materials/DirectionalLightMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
DirectionalLightMaterial::DirectionalLightMaterial(
	std::shared_ptr<Effect> effect,
	const std::string& defaultTechniqueName /*= "main11"*/
	)
	: LightMaterial(effect, defaultTechniqueName)

	, m_lightDirection(effect->GetVariable("lightDirection"))
{}

DirectionalLightMaterial::~DirectionalLightMaterial() = default;
} // namespace library
