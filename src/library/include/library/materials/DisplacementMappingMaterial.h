#pragma once
#include "library/Materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
	class DisplacementMappingMaterial : public Material
	{
		RTTI_CLASS(DisplacementMappingMaterial, Material)

	public:
		using Vertex = VertexPositionTextureNormal;

		explicit DisplacementMappingMaterial(Effect& effect);
		~DisplacementMappingMaterial();

		EffectVariable& GetAmbientColor() { return m_ambientColor; }
		EffectVariable& GetLightColor() { return m_lightColor; }
		EffectVariable& GetLightPosition() { return m_lightPosition; }
		EffectVariable& GetLightRadius() { return m_lightRadius; }
		EffectVariable& GetDisplacementScale() { return m_displacementScale; }
		EffectVariable& GetCameraPosition() { return m_cameraPosition; }

		EffectVariable& GetWVP() { return m_wvp; }
		EffectVariable& GetWorld() { return m_world; }
		EffectVariable& GetSpecularColor() { return m_specularColor; }
		EffectVariable& GetSpecularPower() { return m_specularPower; }

		EffectVariable& GetColorTexture() { return m_colorTexture; }
		EffectVariable& GetDisplacementMap() { return m_displacementMap; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		void InitializeInternal() override;

		EffectVariable& m_ambientColor;
		EffectVariable& m_lightColor;
		EffectVariable& m_lightPosition;
		EffectVariable& m_lightRadius;
		EffectVariable& m_displacementScale;
		EffectVariable& m_cameraPosition;

		EffectVariable& m_wvp;
		EffectVariable& m_world;
		EffectVariable& m_specularColor;
		EffectVariable& m_specularPower;

		EffectVariable& m_colorTexture;
		EffectVariable& m_displacementMap;
	};
} // namespace library
