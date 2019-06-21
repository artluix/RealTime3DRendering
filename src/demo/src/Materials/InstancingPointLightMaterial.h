#pragma once
#include <library/Materials/Material.h>

class InstancingPointLightMaterial : public library::Material
{
	RTTI_CLASS(InstancingPointLightMaterial, library::Material)

public:
	using Vertex = library::VertexPositionTextureNormal;

	explicit InstancingPointLightMaterial(library::Effect& effect);
	~InstancingPointLightMaterial();

	library::EffectVariable& GetAmbientColor() { return m_ambientColor; }
	library::EffectVariable& GetLightColor() { return m_lightColor; }
	library::EffectVariable& GetLightPosition() { return m_lightPosition; }
	library::EffectVariable& GetLightRadius() { return m_lightRadius; }

	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }
	library::EffectVariable& GetViewProjection() { return m_viewProjection; }

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
	library::EffectVariable& m_lightPosition;
	library::EffectVariable& m_lightRadius;

	library::EffectVariable& m_cameraPosition;
	library::EffectVariable& m_viewProjection;

	library::EffectVariable& m_colorTexture;
};
