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
	library::EffectVariable& GetLightData() { return m_lightData; }

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
	library::EffectVariable& m_lightData;

	library::EffectVariable& m_cameraPosition;
	library::EffectVariable& m_viewProjection;

	library::EffectVariable& m_colorTexture;
};
