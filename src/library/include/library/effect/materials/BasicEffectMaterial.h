#pragma once
#include "library/effect/EffectMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	class BasicEffectMaterial : public EffectMaterial
	{
		RTTI_CLASS(BasicEffectMaterial, EffectMaterial)

	public:
		using Vertex = VertexPositionColor;

		explicit BasicEffectMaterial(const Effect& effect);
		~BasicEffectMaterial();

		EffectVariable& GetWorldViewProjection() const { return m_worldViewProjection; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		void InitializeInternal() override;

		EffectVariable& m_worldViewProjection;
	};
} // namespace library
