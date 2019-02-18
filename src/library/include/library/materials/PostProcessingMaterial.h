#pragma once
#include "library/materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
	class PostProcessingMaterial : public Material
	{
		RTTI_CLASS(PostProcessingMaterial, Material)

	public:
		using Vertex = VertexPositionTexture;

		explicit PostProcessingMaterial(const Effect& effect);
		~PostProcessingMaterial();

		EffectVariable& GetSceneTexture() const { return m_sceneTexture; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		void InitializeInternal() override;

		EffectVariable& m_sceneTexture;
	};
} // namespace library
