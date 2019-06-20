#include "Materials/TransparencyMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

TransparencyMappingMaterial::TransparencyMappingMaterial(Effect& effect)
	: PointLightMaterial(effect, "alphaBlendingWithoutFog")

	, m_fogColor(effect.GetVariable("fogColor"))
	, m_fogStart(effect.GetVariable("fogStart"))
	, m_fogRange(effect.GetVariable("fogRange"))

	, m_transparencyMap(effect.GetVariable("TransparencyMap"))
{}

TransparencyMappingMaterial::~TransparencyMappingMaterial() = default;

//-------------------------------------------------------------------------

void TransparencyMappingMaterial::InitializeInternal()
{
	PointLightMaterial::InitializeInternal();

	CreateInputLayout("alphaBlendingWithFog");
}
