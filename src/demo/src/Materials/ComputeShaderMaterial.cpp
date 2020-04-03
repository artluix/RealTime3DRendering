#include "Materials/ComputeShaderMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

ComputeShaderMaterial::ComputeShaderMaterial(Effect& effect)
	: Material(effect, "compute")
	, m_textureSize(effect.GetVariable("TextureSize"))
	, m_blueColor(effect.GetVariable("BlueColor"))
	, m_outputTexture(effect.GetVariable("OutputTexture"))

	, m_colorTexture(effect.GetVariable("ColorTexture"))
{
}

void ComputeShaderMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), "render");
	//CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), "compute");
}
