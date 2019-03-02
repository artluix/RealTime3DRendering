#pragma once
#include "library/materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
	class PointLightMaterial : public Material
	{
		RTTI_CLASS(PointLightMaterial, Material)

	public:
		using Vertex = VertexPositionTextureNormal;

		explicit PointLightMaterial(Effect& effect, const std::string& defaultTechniqueName = "main11");
		~PointLightMaterial();

		EffectVariable& GetAmbientColor() { return m_ambientColor; }
		EffectVariable& GetLightColor() { return m_lightColor; }
		EffectVariable& GetLightPosition() { return m_lightPosition; }
		EffectVariable& GetLightRadius() { return m_lightRadius; }
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
		EffectVariable& m_lightRadius;
		EffectVariable& m_cameraPosition;

		EffectVariable& m_wvp;
		EffectVariable& m_world;
		EffectVariable& m_specularPower;
		EffectVariable& m_specularColor;

		EffectVariable& m_colorTexture;
	};
} // namespace library
