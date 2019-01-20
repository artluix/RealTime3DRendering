#pragma once
#include "library/materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
	class DiffuseLightingMaterial : public Material
	{
		RTTI_CLASS(DiffuseLightingMaterial, Material)

	public:
		using Vertex = VertexPositionTextureNormal;

		explicit DiffuseLightingMaterial(const Effect& effect);
		~DiffuseLightingMaterial();

		EffectVariable& GetAmbientColor() const { return m_ambientColor; }
		EffectVariable& GetLightColor() const { return m_lightColor; }
		EffectVariable& GetLightDirection() const { return m_lightDirection; }

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

		EffectVariable& m_wvp;
		EffectVariable& m_world;

		EffectVariable& m_colorTexture;
	};
} // namespace library
