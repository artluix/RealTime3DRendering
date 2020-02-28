#pragma once
#include "Materials/LightMaterial.h"

class MultipleLightsMaterial : public LightMaterial
{
	RTTI_CLASS(MultipleLightsMaterial, LightMaterial)

public:
	explicit MultipleLightsMaterial(library::Effect& effect);

	library::EffectVariable& GetColorBufferTexture() { return m_colorBufferTexture; }
	library::EffectVariable& GetNormalBufferTexture() { return m_normalBufferTexture; }
	library::EffectVariable& GetWorldPositionBufferTexture() { return m_worldPositionBufferTexture; }
	library::EffectVariable& GetDepthBufferTexture() { return m_depthBufferTexture; }

	library::VertexBufferData CreateVertexBufferData(
		ID3D11Device* const device,
		const library::Mesh& mesh
	) const override;

protected:
	void InitializeInternal() override;

private:
	// for deferred shading
	library::EffectVariable& m_colorBufferTexture;
	library::EffectVariable& m_normalBufferTexture;
	library::EffectVariable& m_worldPositionBufferTexture;
	library::EffectVariable& m_depthBufferTexture;
};
