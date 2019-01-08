#pragma once
#include "library/materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
	class TextureMappingMaterial : public Material
	{
		RTTI_CLASS(TextureMappingMaterial, Material)

	public:
		using Vertex = VertexPositionTexture;

		explicit TextureMappingMaterial();
		~TextureMappingMaterial();

		EffectVariable& GetWVP() const { return *m_wvp; }
		EffectVariable& GetColorTexture() const { return *m_colorTexture; }

		void Initialize(const Effect& effect) override;

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		EffectVariable* m_wvp = nullptr;
		EffectVariable* m_colorTexture = nullptr;
	};

} // namespace library
