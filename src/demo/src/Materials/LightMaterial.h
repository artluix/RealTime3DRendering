#pragma once
#include <library/Materials/Material.h>

#include <library/Components/SpotlightComponent.h>

#define MAX_LIGHTS_COUNT 4

// move it somewhere else
struct LightsData
{
	std::array<library::DirectionalLightData, MAX_LIGHTS_COUNT> dirLights;
	std::array<library::PointLightData, MAX_LIGHTS_COUNT> pointLights;
	std::array<library::SpotlightData, MAX_LIGHTS_COUNT> spotlights;

	unsigned int dirLightsCount = 0;
	unsigned int pointLightsCount = 0;
	unsigned int spotlightsCount = 0;
};

//-------------------------------------------------------------------------

class LightMaterial : public library::Material
{
	RTTI_CLASS(LightMaterial, library::Material)

public:
	using Vertex = library::VertexPositionTextureNormal;

	~LightMaterial();

	library::EffectVariable& GetAmbientColor() { return m_ambientColor; }
	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }

	library::EffectVariable& GetLightData() { return m_lightData; }
	library::EffectVariable& GetLightsData() { return m_lightsData; }

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
	explicit LightMaterial(library::Effect& effect, const std::string& defaultTechniqueName = "main11");

	void InitializeInternal() override;

private:
	library::EffectVariable& m_ambientColor;
	library::EffectVariable& m_cameraPosition;

	library::EffectVariable& m_lightData;
	library::EffectVariable& m_lightsData;

	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;
	library::EffectVariable& m_specularPower;
	library::EffectVariable& m_specularColor;

	library::EffectVariable& m_colorTexture;
};
