#pragma once
#include <library/Materials/Material.h>

class TextureMappingMaterial : public library::Material
{
	RTTI_CLASS(TextureMappingMaterial, library::Material)

public:
	using Vertex = library::VertexPositionTexture;

	explicit TextureMappingMaterial(library::Effect& effect);
	~TextureMappingMaterial();

	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_wvp;
	library::EffectVariable& m_colorTexture;
};
