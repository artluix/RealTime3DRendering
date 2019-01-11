#pragma once
#include "library/effect/EffectMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	class DiffuseLightingEffectMaterial : public EffectMaterial
	{
		RTTI_CLASS(DiffuseLightingEffectMaterial, EffectMaterial)

	public:
		using Vertex = VertexPositionTextureNormal;

		explicit DiffuseLightingEffectMaterial(const Effect& effect);
		~DiffuseLightingEffectMaterial();

		EffectVariable& GetWVP() const { return m_wvp; }
		EffectVariable& GetWorld() const { return m_world; }
		EffectVariable& GetAmbientColor() const { return m_ambientColor; }
		EffectVariable& GetLightColor() const { return m_lightColor; }
		EffectVariable& GetLightDirection() const { return m_lightDirection; }
		EffectVariable& GetColorTexture() const { return m_colorTexture; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		void InitializeInternal() override;

		EffectVariable& m_wvp;
		EffectVariable& m_world;
		EffectVariable& m_ambientColor;
		EffectVariable& m_lightColor;
		EffectVariable& m_lightDirection;
		EffectVariable& m_colorTexture;
	};

} // namespace library
