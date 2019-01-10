#pragma once
#include "library/effect/EffectMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	namespace effect
	{
		namespace materials
		{
			class Basic : public Material
			{
				RTTI_CLASS(Basic, Material)

			public:
				using Vertex = VertexPositionColor;

				explicit Basic(const Effect& effect);
				~Basic();

				Variable& GetWorldViewProjection() const { return m_worldViewProjection; }

				ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
				unsigned GetVertexSize() const override { return sizeof(Vertex); }

			private:
				void InitializeInternal() override;

				Variable& m_worldViewProjection;
			};

		} // namespace materials
	} // namespace effect
} // namespace library
