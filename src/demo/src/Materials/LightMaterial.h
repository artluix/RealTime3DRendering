#pragma once
#include "LightsData.h"

#include <library/Materials/Material.h>

class LightMaterial : public library::Material
{
	RTTI_CLASS(LightMaterial, library::Material)

public:
	using Vertex = library::VertexPositionTextureNormal;

	explicit LightMaterial(library::Effect& effect, const std::string& defaultTechniqueName = "main11");
	~LightMaterial();

	library::EffectVariable& GetAmbientColor() { return m_ambientColor; }
	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }

	library::EffectVariable& GetDirLights() { return m_dirLights; }
	library::EffectVariable& GetDirLightsCount() { return m_dirLightsCount; }

	library::EffectVariable& GetPointLights() { return m_pointLights; }
	library::EffectVariable& GetPointLightsCount() { return m_pointLightsCount; }

	library::EffectVariable& GetSpotlights() { return m_spotlights; }
	library::EffectVariable& GetSpotlightsCount() { return m_spotlightsCount; }

	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }

	library::EffectVariable& GetSpecularPower() { return m_specularPower; }
	library::EffectVariable& GetSpecularColor() { return m_specularColor; }

	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

	library::VertexBufferData CreateVertexBufferData(
		ID3D11Device* const device,
		const library::Mesh& mesh
	) const override;

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_ambientColor;
	library::EffectVariable& m_cameraPosition;

	library::EffectVariable& m_dirLights;
	library::EffectVariable& m_dirLightsCount;

	library::EffectVariable& m_pointLights;
	library::EffectVariable& m_pointLightsCount;

	library::EffectVariable& m_spotlights;
	library::EffectVariable& m_spotlightsCount;

	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;
	library::EffectVariable& m_specularPower;
	library::EffectVariable& m_specularColor;

	library::EffectVariable& m_colorTexture;
};
