#pragma once
#include "library/effect/EffectMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	class TransparencyMappingEffectMaterial : public EffectMaterial
	{
		RTTI_CLASS(TransparencyMappingEffectMaterial, EffectMaterial)

	public:
		using Vertex = VertexPositionTextureNormal;

		explicit TransparencyMappingEffectMaterial(const Effect& effect);
		~TransparencyMappingEffectMaterial();

		EffectVariable& GetAmbientColor() const { return m_ambientColor; }
		EffectVariable& GetLightColor() const { return m_lightColor; }
		EffectVariable& GetLightPosition() const { return m_lightPosition; }
		EffectVariable& GetLightRadius() const { return m_lightRadius; }
		EffectVariable& GetFogColor() const { return m_fogColor; }
		EffectVariable& GetFogStart() const { return m_fogStart; }
		EffectVariable& GetFogRange() const { return m_fogRange; }
		EffectVariable& GetCameraPosition() const { return m_cameraPosition; }

		EffectVariable& GetWVP() const { return m_wvp; }
		EffectVariable& GetWorld() const { return m_world; }
		EffectVariable& GetSpecularColor() const { return m_specularColor; }
		EffectVariable& GetSpecularPower() const { return m_specularPower; }

		EffectVariable& GetColorTexture() const { return m_colorTexture; }
		EffectVariable& GetTransparencyMap() const { return m_transparencyMap; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		void InitializeInternal() override;

		EffectVariable& m_ambientColor;
		EffectVariable& m_lightColor;
		EffectVariable& m_lightPosition;
		EffectVariable& m_lightRadius;
		EffectVariable& m_fogColor;
		EffectVariable& m_fogStart;
		EffectVariable& m_fogRange;
		EffectVariable& m_cameraPosition;

		EffectVariable& m_wvp;
		EffectVariable& m_world;
		EffectVariable& m_specularColor;
		EffectVariable& m_specularPower;

		EffectVariable& m_colorTexture;
		EffectVariable& m_transparencyMap;
	};
} // namespace library
