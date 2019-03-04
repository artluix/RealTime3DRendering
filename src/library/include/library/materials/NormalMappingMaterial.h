#pragma once
#include "library/Materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
	class NormalMappingMaterial : public Material
	{
		RTTI_CLASS(NormalMappingMaterial, Material)

	public:
		using Vertex = VertexPositionTextureNormalTangent;

		explicit NormalMappingMaterial(Effect& effect);
		~NormalMappingMaterial();

		EffectVariable& GetAmbientColor() { return m_ambientColor; }
		EffectVariable& GetLightColor() { return m_lightColor; }
		EffectVariable& GetLightDirection() { return m_lightDirection; }
		EffectVariable& GetCameraPosition() { return m_cameraPosition; }

		EffectVariable& GetWVP() { return m_wvp; }
		EffectVariable& GetWorld() { return m_world; }
		EffectVariable& GetSpecularPower() { return m_specularPower; }
		EffectVariable& GetSpecularColor() { return m_specularColor; }

		EffectVariable& GetColorTexture() { return m_colorTexture; }
		EffectVariable& GetNormalMap() { return m_normalMap; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		void InitializeInternal() override;

		EffectVariable& m_ambientColor;
		EffectVariable& m_lightColor;
		EffectVariable& m_lightDirection;
		EffectVariable& m_cameraPosition;

		EffectVariable& m_wvp;
		EffectVariable& m_world;
		EffectVariable& m_specularPower;
		EffectVariable& m_specularColor;

		EffectVariable& m_colorTexture;
		EffectVariable& m_normalMap;
	};
} // namespace library
