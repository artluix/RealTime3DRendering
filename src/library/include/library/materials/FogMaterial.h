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

		explicit FogMaterial(Effect& effect);
		~FogMaterial();

		EffectVariable& GetAmbientColor() { return m_ambientColor; }
		EffectVariable& GetLightColor() { return m_lightColor; }
		EffectVariable& GetLightDirection() { return m_lightDirection; }
		EffectVariable& GetFogColor() { return m_fogColor; }
		EffectVariable& GetFogStart() { return m_fogStart; }
		EffectVariable& GetFogRange() { return m_fogRange; }
		EffectVariable& GetCameraPosition() { return m_cameraPosition; }

		EffectVariable& GetWVP() { return m_wvp; }
		EffectVariable& GetWorld() { return m_world; }

		EffectVariable& GetColorTexture() { return m_colorTexture; }

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
