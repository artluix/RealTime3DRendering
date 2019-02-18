#pragma once
#include "library/materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
	class SpotlightMaterial : public Material
	{
		RTTI_CLASS(SpotlightMaterial, Material)

	public:
		using Vertex = VertexPositionTextureNormal;

		explicit SpotlightMaterial(const Effect& effect);
		~SpotlightMaterial();

		EffectVariable& GetAmbientColor() const { return m_ambientColor; }
		EffectVariable& GetLightColor() const { return m_lightColor; }
		EffectVariable& GetLightPosition() const { return m_lightPosition; }
		EffectVariable& GetLightLookAt() const { return m_lightLookAt; }
		EffectVariable& GetLightRadius() const { return m_lightRadius; }
		EffectVariable& GetSpotlightInnerAngle() const { return m_spotlightInnerAngle; }
		EffectVariable& GetSpotlightOuterAngle() const { return m_spotlightOuterAngle; }
		EffectVariable& GetCameraPosition() const { return m_cameraPosition; }

		EffectVariable& GetWVP() const { return m_wvp; }
		EffectVariable& GetWorld() const { return m_world; }
		EffectVariable& GetSpecularPower() const { return m_specularPower; }
		EffectVariable& GetSpecularColor() const { return m_specularColor; }

		EffectVariable& GetColorTexture() const { return m_colorTexture; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		void InitializeInternal() override;

		EffectVariable& m_ambientColor;
		EffectVariable& m_lightColor;
		EffectVariable& m_lightPosition;
		EffectVariable& m_lightLookAt;
		EffectVariable& m_lightRadius;
		EffectVariable& m_spotlightInnerAngle;
		EffectVariable& m_spotlightOuterAngle;
		EffectVariable& m_cameraPosition;

		EffectVariable& m_wvp;
		EffectVariable& m_world;
		EffectVariable& m_specularPower;
		EffectVariable& m_specularColor;

		EffectVariable& m_colorTexture;
	};
} // namespace library