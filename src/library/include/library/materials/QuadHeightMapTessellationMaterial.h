#pragma once
#include "library/Materials/TessellationMaterial.h"

namespace library
{
class QuadHeightMapTessellationMaterial : public TessellationMaterial
{
	RTTI_CLASS(QuadHeightMapTessellationMaterial, TessellationMaterial)

public:
	using Vertex = VertexPositionTexture;

	explicit QuadHeightMapTessellationMaterial(Effect& effect);

	EffectVariable& GetTextureMatrix() { return m_textureMatrix; }
	EffectVariable& GetHeightMapTexture() { return m_heightMapTexture; }
	EffectVariable& GetDisplacementScale() { return m_displacementScale; }

	unsigned GetVertexSize() const override { return sizeof(Vertex); }

	VertexBufferData CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const override;

protected:
	void InitializeInternal() override;

	EffectVariable& m_textureMatrix;
	EffectVariable& m_displacementScale;
	EffectVariable& m_heightMapTexture;
};
} // namespace library
