#pragma once
#include "library/materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
	class TransparencyMappingMaterial : public Material
	{
		RTTI_CLASS(TransparencyMappingMaterial, Material)

	public:
		using Vertex = VertexPositionTextureNormal;

		explicit TransparencyMappingMaterial(Effect& effect);
		~TransparencyMappingMaterial();

		EffectVariable& GetAmbientColor() { return m_ambientColor; }
		EffectVariable& GetLightColor() { return m_lightColor; }
		EffectVariable& GetLightPosition() { return m_lightPosition; }
		EffectVariable& GetLightRadius() { return m_lightRadius; }
		EffectVariable& GetFogColor() { return m_fogColor; }
		EffectVariable& GetFogStart() { return m_fogStart; }
		EffectVariable& GetFogRange() { return m_fogRange; }
		EffectVariable& GetCameraPosition() { return m_cameraPosition; }

		EffectVariable& GetWVP() { return m_wvp; }
		EffectVariable& GetWorld() { return m_world; }
		EffectVariable& GetSpecularColor() { return m_specularColor; }
		EffectVariable& GetSpecularPower() { return m_specularPower; }

		EffectVariable& GetColorTexture() { return m_colorTexture; }
		EffectVariable& GetTransparencyMap() { return m_transparencyMap; }

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
