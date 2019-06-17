#pragma once
#include "library/Materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
class TextureMappingMaterial : public Material
{
	RTTI_CLASS(TextureMappingMaterial, Material)

public:
	using Vertex = VertexPositionTexture;

	explicit TextureMappingMaterial(Effect& effect);
	~TextureMappingMaterial();

	EffectVariable& GetWVP() { return m_wvp; }
	EffectVariable& GetColorTexture() { return m_colorTexture; }

	VertexBufferData CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const override;
	unsigned GetVertexSize() const override { return sizeof(Vertex); }

protected:
	void InitializeInternal() override;

private:
	EffectVariable& m_wvp;
	EffectVariable& m_colorTexture;
};
} // namespace library
