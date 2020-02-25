#pragma once
#include "Materials/LightMaterial.h"

class MultiplePointLightsMaterial : public LightMaterial
{
	RTTI_CLASS(MultiplePointLightsMaterial, LightMaterial)

public:
	explicit MultiplePointLightsMaterial(library::Effect& effect);

	library::EffectVariable& GetLightsData() { return m_lightsData; }
	library::EffectVariable& GetLightsCount() { return m_lightsCount; }

	//-------------------------------------------------------------------------

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
	library::EffectVariable& m_lightsData;
	library::EffectVariable& m_lightData;
	library::EffectVariable& m_lightsCount;

	// for deferred shading
	library::EffectVariable& m_colorBufferTexture;
	library::EffectVariable& m_normalBufferTexture;
	library::EffectVariable& m_worldPositionBufferTexture;
	library::EffectVariable& m_depthBufferTexture;
};
