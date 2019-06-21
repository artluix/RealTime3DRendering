#pragma once
#include "Materials/TessellationMaterial.h"

class QuadHeightMapTessellationMaterial : public TessellationMaterial
{
	RTTI_CLASS(QuadHeightMapTessellationMaterial, TessellationMaterial)

public:
	using Vertex = library::VertexPositionTexture;

	explicit QuadHeightMapTessellationMaterial(library::Effect& effect);

	library::EffectVariable& GetTextureMatrix() { return m_textureMatrix; }
	library::EffectVariable& GetHeightMapTexture() { return m_heightMapTexture; }
	library::EffectVariable& GetDisplacementScale() { return m_displacementScale; }

	library::VertexBufferData CreateVertexBufferData(
		ID3D11Device* const device,
		const library::Mesh& mesh
	) const override;

protected:
	void InitializeInternal() override;

	library::EffectVariable& m_textureMatrix;
	library::EffectVariable& m_displacementScale;
	library::EffectVariable& m_heightMapTexture;
};
