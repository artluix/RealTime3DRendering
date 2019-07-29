#pragma once
#include <library/Materials/Material.h>

struct PointLight
{
	library::math::Vector3 position;
	float lightRadius;
	library::math::Color color;
};

//-------------------------------------------------------------------------

class MultiplePointLightsMaterial : public library::Material
{
	RTTI_CLASS(MultiplePointLightsMaterial, library::Material)

public:
	using Vertex = library::VertexPositionTextureNormal;

	explicit MultiplePointLightsMaterial(library::Effect& effect);

	library::EffectVariable& GetPointLights() { return m_pointLights; }

	library::EffectVariable& GetLightsCount() { return m_lightsCount; }
	library::EffectVariable& GetAmbientColor() { return m_ambientColor; }
	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }

	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }
	library::EffectVariable& GetSpecularPower() { return m_specularPower; }
	library::EffectVariable& GetSpecularColor() { return m_specularColor; }

	library::EffectVariable& GetModelTexture() { return m_modelTexture; }

	library::EffectVariable& GetColorTexture() { return m_colorTexture; }
	library::EffectVariable& GetNormalTexture() { return m_normalTexture; }

	library::VertexBufferData CreateVertexBufferData(
		ID3D11Device* const device,
		const library::Mesh& mesh
	) const override;

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_pointLights;

	library::EffectVariable& m_lightsCount;
	library::EffectVariable& m_ambientColor;
	library::EffectVariable& m_cameraPosition;

	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;
	library::EffectVariable& m_specularPower;
	library::EffectVariable& m_specularColor;

	library::EffectVariable& m_modelTexture;

	// for deferred shading
	library::EffectVariable& m_colorTexture;
	library::EffectVariable& m_normalTexture;
};
