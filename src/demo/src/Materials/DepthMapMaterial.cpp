#include "Materials/DepthMapMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

DepthMapMaterial::DepthMapMaterial(Effect& effect)
	: Material(effect, "create_depth_map")
	, m_worldLightViewProjection(effect.GetVariable("WorldLightViewProjection"))
{}

DepthMapMaterial::~DepthMapMaterial() = default;

//-------------------------------------------------------------------------

void DepthMapMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), "depth_map_bias");
}
