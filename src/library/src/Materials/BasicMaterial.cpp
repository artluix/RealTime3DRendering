#include "StdAfx.h"
#include "library/Materials/BasicMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
BasicMaterial::BasicMaterial(Effect& effect)
	: Material(effect, "main11")
	, m_worldViewProjection(effect.GetVariable("WorldViewProjection"))
{}

BasicMaterial::~BasicMaterial() = default;

//-------------------------------------------------------------------------

void BasicMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
}
} // namespace library
