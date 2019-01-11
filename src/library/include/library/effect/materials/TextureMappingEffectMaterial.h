#pragma once
#include "library/effect/EffectMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	class TextureMappingEffectMaterial : public EffectMaterial
	{
		RTTI_CLASS(TextureMappingEffectMaterial, EffectMaterial)

	public:
		using Vertex = VertexPositionTexture;

		explicit TextureMappingEffectMaterial(const Effect& effect);
		~TextureMappingEffectMaterial();

		EffectVariable& GetWVP() const { return m_wvp; }
		EffectVariable& GetColorTexture() const { return m_colorTexture; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		void InitializeInternal() override;

		EffectVariable& m_wvp;
		EffectVariable& m_colorTexture;
	};
} // namespace library
