#pragma once
#include "library/materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
	class BasicMaterial : public Material
	{
		RTTI_CLASS(BasicMaterial, Material)

	public:
		using Vertex = VertexPositionColor;

		explicit BasicMaterial(const Effect& effect);
		~BasicMaterial();

		EffectVariable& GetWorldViewProjection() const { return m_worldViewProjection; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		void InitializeInternal() override;

		EffectVariable& m_worldViewProjection;
	};
} // namespace library
