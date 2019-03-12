#include "StdAfx.h"
#include "library/Materials/PointLightMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
	PointLightMaterial::PointLightMaterial(
		Effect& effect,
		const std::string& defaultTechniqueName /*= "main11"*/
	)
		: LightMaterial(effect, defaultTechniqueName)

		, m_lightPosition(effect.GetVariable("lightPosition"))
		, m_lightRadius(effect.GetVariable("lightRadius"))
	{
	}

	PointLightMaterial::~PointLightMaterial() = default;
} // namespace library
