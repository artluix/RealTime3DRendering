#pragma once
#include <library/Materials/Material.h>

class ComputeShaderMaterial : public library::Material
{
	RTTI_CLASS(ComputeShaderMaterial, library::Material)

public:
	using Vertex = library::VertexPositionTexture;

	explicit ComputeShaderMaterial(library::Effect& effect);

	library::EffectVariable& GetTextureSize() { return m_textureSize; }
	library::EffectVariable& GetBlueColor() { return m_blueColor; }
	library::EffectVariable& GetOutputTexture() { return m_outputTexture; }

	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

	library::VertexBufferData CreateVertexBufferData(
		ID3D11Device* const device,
		const library::Mesh& mesh
	) const override;

protected:
	void InitializeInternal() override;

private:
	// compute shader variables
	library::EffectVariable& m_textureSize;
	library::EffectVariable& m_blueColor;
	library::EffectVariable& m_outputTexture;

	// rendering shader variables
	library::EffectVariable& m_colorTexture;
};
