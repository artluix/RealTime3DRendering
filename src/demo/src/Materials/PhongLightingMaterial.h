#pragma once
#include "Materials/LightsMaterial.h"

class PhongLightingMaterial : public LightsMaterial
{
	RTTI_CLASS(PhongLightingMaterial, LightsMaterial)

public:
	using Vertex = library::VertexPositionTextureNormal;

	explicit PhongLightingMaterial(library::Effect& effect, const std::string& defaultTechniqueName = "forward");

	library::EffectVariable& GetAmbientColor() { return m_ambientColor; }
	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }

	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }

	library::EffectVariable& GetSpecularPower() { return m_specularPower; }
	library::EffectVariable& GetSpecularColor() { return m_specularColor; }

	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

	// deferred part
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
	library::EffectVariable& m_ambientColor;
	library::EffectVariable& m_cameraPosition;

	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;
	library::EffectVariable& m_specularPower;
	library::EffectVariable& m_specularColor;

	library::EffectVariable& m_colorTexture;

	// for deferred shading
	library::EffectVariable& m_colorBufferTexture;
	library::EffectVariable& m_normalBufferTexture;
	library::EffectVariable& m_worldPositionBufferTexture;
	library::EffectVariable& m_depthBufferTexture;
};
