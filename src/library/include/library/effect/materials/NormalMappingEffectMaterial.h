#pragma once
#include "library/effect/EffectMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	class NormalMappingEffectMaterial : public EffectMaterial
	{
		RTTI_CLASS(NormalMappingEffectMaterial, EffectMaterial)

	public:
		using Vertex = VertexPositionTextureNormalTangent;

		explicit NormalMappingEffectMaterial(const Effect& effect);
		~NormalMappingEffectMaterial();

		EffectVariable& GetWVP() const { return m_wvp; }
		EffectVariable& GetWorld() const { return m_world; }
		EffectVariable& GetSpecularPower() const { return m_specularPower; }
		EffectVariable& GetSpecularColor() const { return m_specularColor; }
		EffectVariable& GetAmbientColor() const { return m_ambientColor; }
		EffectVariable& GetLightColor() const { return m_lightColor; }
		EffectVariable& GetLightDirection() const { return m_lightDirection; }
		EffectVariable& GetCameraPosition() const { return m_cameraPosition; }
		EffectVariable& GetColorTexture() const { return m_colorTexture; }
		EffectVariable& GetNormalMap() const { return m_normalMap; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		void InitializeInternal() override;

		EffectVariable& m_wvp;
		EffectVariable& m_world;
		EffectVariable& m_specularPower;
		EffectVariable& m_specularColor;
		EffectVariable& m_ambientColor;
		EffectVariable& m_lightColor;
		EffectVariable& m_lightDirection;
		EffectVariable& m_cameraPosition;
		EffectVariable& m_colorTexture;
		EffectVariable& m_normalMap;
	};
} // namespace library
