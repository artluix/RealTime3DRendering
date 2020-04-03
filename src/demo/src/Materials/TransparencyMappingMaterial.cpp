#include "Materials/TransparencyMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

TransparencyMappingMaterial::TransparencyMappingMaterial(Effect& effect)
	: FogMaterial(effect, "alphaBlendingWithoutFog")

	, m_transparencyMap(effect.GetVariable("TransparencyMap"))
{}

TransparencyMappingMaterial::~TransparencyMappingMaterial() = default;

//-------------------------------------------------------------------------

void TransparencyMappingMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), "alphaBlendingWithFog");
}
