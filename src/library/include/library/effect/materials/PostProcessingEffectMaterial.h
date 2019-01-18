#pragma once
#include "library/effect/EffectMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	class PostProcessingEffectMaterial : public EffectMaterial
	{
		RTTI_CLASS(PostProcessingEffectMaterial, EffectMaterial)

	public:
		using Vertex = VertexPositionTexture;

		explicit PostProcessingEffectMaterial(const Effect& effect);
		~PostProcessingEffectMaterial();

		EffectVariable& GetColorTexture() const { return m_colorTexture; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		void InitializeInternal() override;

		EffectVariable& m_colorTexture;
	};
} // namespace library
