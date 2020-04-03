#include "Materials/TextureMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

TextureMappingMaterial::TextureMappingMaterial(Effect& effect)
	: Material(effect, "main11")
	, m_wvp(effect.GetVariable("WVP"))
	, m_colorTexture(effect.GetVariable("ColorTexture"))
{}

TextureMappingMaterial::~TextureMappingMaterial() = default;

void TextureMappingMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
}
