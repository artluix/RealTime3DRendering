#pragma once
#include <library/Materials/Material.h>

class LightMaterial : public library::Material
{
	RTTI_CLASS(LightMaterial, library::Material)

public:
	using Vertex = library::VertexPositionTextureNormal;

	~LightMaterial();

	library::EffectVariable& GetAmbientColor() { return m_ambientColor; }
	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }

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

	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;
	library::EffectVariable& m_specularPower;
	library::EffectVariable& m_specularColor;

	library::EffectVariable& m_colorTexture;
};
