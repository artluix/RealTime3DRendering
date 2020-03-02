#include "Materials/FogMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

FogMaterial::FogMaterial(Effect& effect)
	: LightsMaterial(effect, "fogEnabled")

	, m_fogColor(effect.GetVariable("FogColor"))
	, m_fogStart(effect.GetVariable("FogStart"))
	, m_fogRange(effect.GetVariable("FogRange"))
{}

FogMaterial::~FogMaterial() = default;

//-------------------------------------------------------------------------

void FogMaterial::InitializeInternal()
{
	LightsMaterial::InitializeInternal();

	const auto& inputElementDescriptions = GetCurrentPass().GetInputElementDescriptions();

	CreateInputLayout(inputElementDescriptions, "fogDisabled");
}
