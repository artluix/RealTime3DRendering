#include "Materials/FogMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

FogMaterial::FogMaterial(Effect& effect)
	: DirectionalLightMaterial(effect, "fogEnabled")

	, m_fogColor(effect.GetVariable("fogColor"))
	, m_fogStart(effect.GetVariable("fogStart"))
	, m_fogRange(effect.GetVariable("fogRange"))
{}

FogMaterial::~FogMaterial() = default;

//-------------------------------------------------------------------------

void FogMaterial::InitializeInternal()
{
	DirectionalLightMaterial::InitializeInternal();

	CreateInputLayout("fogDisabled");
}
