#include "StdAfx.h"
#include "library/Materials/FogMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
FogMaterial::FogMaterial(std::shared_ptr<Effect> effect)
	: DirectionalLightMaterial(effect, "fogEnabled")

	, m_fogColor(effect->GetVariable("fogColor"))
	, m_fogStart(effect->GetVariable("fogStart"))
	, m_fogRange(effect->GetVariable("fogRange"))
{}

FogMaterial::~FogMaterial() = default;
} // namespace library
