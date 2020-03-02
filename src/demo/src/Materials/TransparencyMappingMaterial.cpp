#include "Materials/TransparencyMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

TransparencyMappingMaterial::TransparencyMappingMaterial(Effect& effect)
	: LightsMaterial(effect, "alphaBlendingWithoutFog")

	, m_fogColor(effect.GetVariable("FogColor"))
	, m_fogStart(effect.GetVariable("FogStart"))
	, m_fogRange(effect.GetVariable("FogRange"))

	, m_transparencyMap(effect.GetVariable("TransparencyMap"))
{}

TransparencyMappingMaterial::~TransparencyMappingMaterial() = default;

//-------------------------------------------------------------------------

void TransparencyMappingMaterial::InitializeInternal()
{
	LightsMaterial::InitializeInternal();

	const auto& inputElementDescriptions = GetCurrentPass().GetInputElementDescriptions();

	CreateInputLayout(inputElementDescriptions, "alphaBlendingWithFog");
}
