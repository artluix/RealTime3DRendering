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

		explicit SpotlightMaterial(Effect& effect);
		~SpotlightMaterial();

		EffectVariable& GetAmbientColor() { return m_ambientColor; }
		EffectVariable& GetLightColor() { return m_lightColor; }
		EffectVariable& GetLightPosition() { return m_lightPosition; }
		EffectVariable& GetLightLookAt() { return m_lightLookAt; }
		EffectVariable& GetLightRadius() { return m_lightRadius; }
		EffectVariable& GetSpotlightInnerAngle() { return m_spotlightInnerAngle; }
		EffectVariable& GetSpotlightOuterAngle() { return m_spotlightOuterAngle; }
		EffectVariable& GetCameraPosition() { return m_cameraPosition; }

		EffectVariable& GetWVP() { return m_wvp; }
		EffectVariable& GetWorld() { return m_world; }
		EffectVariable& GetSpecularPower() { return m_specularPower; }
		EffectVariable& GetSpecularColor() { return m_specularColor; }

		EffectVariable& GetColorTexture() { return m_colorTexture; }

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
