#pragma once
#include "library/effect/EffectMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	class SkyboxEffectMaterial : public EffectMaterial
	{
		RTTI_CLASS(SkyboxEffectMaterial, EffectMaterial)

	public:
		using Vertex = VertexPosition;

		explicit SkyboxEffectMaterial(const Effect& effect);
		~SkyboxEffectMaterial();

		EffectVariable& GetWorldViewProjection() const { return m_worldViewProjection; }
		EffectVariable& GetSkyboxTexture() const { return m_skyboxTexture; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		void InitializeInternal() override;

		EffectVariable& m_worldViewProjection;
		EffectVariable& m_skyboxTexture;
	};
} // namespace library
