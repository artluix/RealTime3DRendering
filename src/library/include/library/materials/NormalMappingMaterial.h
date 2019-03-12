#pragma once
#include "library/Materials/DirectionalLightMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	class NormalMappingMaterial : public DirectionalLightMaterial
	{
		RTTI_CLASS(NormalMappingMaterial, DirectionalLightMaterial)

	public:
		using Vertex = VertexPositionTextureNormalTangent;

		explicit NormalMappingMaterial(Effect& effect);
		~NormalMappingMaterial();

		EffectVariable& GetNormalMap() { return m_normalMap; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	protected:
		void InitializeInternal() override;

	private:
		EffectVariable& m_normalMap;
	};
} // namespace library
