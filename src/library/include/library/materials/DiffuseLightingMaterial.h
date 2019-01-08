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

		explicit DiffuseLightingMaterial();
		~DiffuseLightingMaterial();

		EffectVariable& GetWVP() const { return *m_wvp; }
		EffectVariable& GetWorld() const { return *m_world; }
		EffectVariable& GetAmbientColor() const { return *m_ambientColor; }
		EffectVariable& GetLightColor() const { return *m_lightColor; }
		EffectVariable& GetLightDirection() const { return *m_lightDirection; }
		EffectVariable& GetColorTexture() const { return *m_colorTexture; }

		void Initialize(const Effect& effect) override;

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		EffectVariable* m_wvp = nullptr;
		EffectVariable* m_world = nullptr;
		EffectVariable* m_ambientColor = nullptr;
		EffectVariable* m_lightColor = nullptr;
		EffectVariable* m_lightDirection = nullptr;
		EffectVariable* m_colorTexture = nullptr;
	};

} // namespace library
