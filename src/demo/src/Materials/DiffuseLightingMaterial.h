#pragma once
#include <library/Materials/Material.h>

class DiffuseLightingMaterial : public library::Material
{
	RTTI_CLASS(DiffuseLightingMaterial, library::Material)

public:
	using Vertex = library::VertexPositionTextureNormal;

	explicit DiffuseLightingMaterial(library::Effect& effect);
	~DiffuseLightingMaterial();

	library::EffectVariable& GetAmbientColor() { return m_ambientColor; }
	library::EffectVariable& GetLightColor() { return m_lightColor; }
	library::EffectVariable& GetLightDirection() { return m_lightDirection; }

	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }

	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

	library::VertexBufferData CreateVertexBufferData(
		ID3D11Device* const device,
		const library::Mesh& mesh
	) const override;

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_ambientColor;
	library::EffectVariable& m_lightColor;
	library::EffectVariable& m_lightDirection;

	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;

	library::EffectVariable& m_colorTexture;
};
