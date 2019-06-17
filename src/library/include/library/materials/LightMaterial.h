#pragma once
#include "library/Materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
class LightMaterial : public Material
{
	RTTI_CLASS(LightMaterial, Material)

public:
	using Vertex = VertexPositionTextureNormal;

	~LightMaterial();

	EffectVariable& GetAmbientColor() { return m_ambientColor; }
	EffectVariable& GetLightColor() { return m_lightColor; }
	EffectVariable& GetCameraPosition() { return m_cameraPosition; }

	EffectVariable& GetWVP() { return m_wvp; }
	EffectVariable& GetWorld() { return m_world; }
	EffectVariable& GetSpecularPower() { return m_specularPower; }
	EffectVariable& GetSpecularColor() { return m_specularColor; }

	EffectVariable& GetColorTexture() { return m_colorTexture; }

	VertexBufferData CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const override;
	unsigned GetVertexSize() const override { return sizeof(Vertex); }

protected:
	explicit LightMaterial(Effect& effect, const std::string& defaultTechniqueName = "main11");

	void InitializeInternal() override;

private:
	EffectVariable& m_ambientColor;
	EffectVariable& m_lightColor;
	EffectVariable& m_cameraPosition;

	EffectVariable& m_wvp;
	EffectVariable& m_world;
	EffectVariable& m_specularPower;
	EffectVariable& m_specularColor;

	EffectVariable& m_colorTexture;
};
} // namespace library
