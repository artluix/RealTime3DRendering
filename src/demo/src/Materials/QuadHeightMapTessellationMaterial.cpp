#include "Materials/QuadHeightMapTessellationMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

QuadHeightMapTessellationMaterial::QuadHeightMapTessellationMaterial(Effect& effect)
	: TessellationMaterial(effect)

	, m_textureMatrix(effect.GetVariable("TextureMatrix"))
	, m_displacementScale(effect.GetVariable("DisplacementScale"))
	, m_heightMapTexture(effect.GetVariable("HeightMap"))
{}

//-------------------------------------------------------------------------

void QuadHeightMapTessellationMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
}
