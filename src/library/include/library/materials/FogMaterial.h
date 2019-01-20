#pragma once
#include "library/materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
	class FogMaterial : public Material
	{
		RTTI_CLASS(FogMaterial, Material)

	public:
		using Vertex = VertexPositionTextureNormal;

		explicit FogMaterial(const Effect& effect);
		~FogMaterial();

		EffectVariable& GetAmbientColor() const { return m_ambientColor; }
		EffectVariable& GetLightColor() const { return m_lightColor; }
		EffectVariable& GetLightDirection() const { return m_lightDirection; }
		EffectVariable& GetFogColor() const { return m_fogColor; }
		EffectVariable& GetFogStart() const { return m_fogStart; }
		EffectVariable& GetFogRange() const { return m_fogRange; }
		EffectVariable& GetCameraPosition() const { return m_cameraPosition; }

		EffectVariable& GetWVP() const { return m_wvp; }
		EffectVariable& GetWorld() const { return m_world; }

		EffectVariable& GetColorTexture() const { return m_colorTexture; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		void InitializeInternal() override;

		EffectVariable& m_ambientColor;
		EffectVariable& m_lightColor;
		EffectVariable& m_lightDirection;
		EffectVariable& m_fogColor;
		EffectVariable& m_fogStart;
		EffectVariable& m_fogRange;
		EffectVariable& m_cameraPosition;

		EffectVariable& m_wvp;
		EffectVariable& m_world;

		EffectVariable& m_colorTexture;
	};
} // namespace library
