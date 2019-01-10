#pragma once
#include "library/effect/EffectMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	namespace effect
	{
		namespace materials
		{
			class TextureMapping : public Material
			{
				RTTI_CLASS(TextureMapping, Material)

			public:
				using Vertex = VertexPositionTexture;

				explicit TextureMapping(const Effect& effect);
				~TextureMapping();

				Variable& GetWVP() const { return m_wvp; }
				Variable& GetColorTexture() const { return m_colorTexture; }

				ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
				unsigned GetVertexSize() const override { return sizeof(Vertex); }

			private:
				void InitializeInternal() override;

				Variable& m_wvp;
				Variable& m_colorTexture;
			};

		} // namespace materials
	} // namespace effect
} // namespace library
