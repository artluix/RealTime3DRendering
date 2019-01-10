#pragma once
#include "library/effect/EffectMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	namespace effect
	{
		namespace materials
		{
			class DiffuseLighting : public Material
			{
				RTTI_CLASS(DiffuseLighting, Material)

			public:
				using Vertex = VertexPositionTextureNormal;

				explicit DiffuseLighting(const Effect& effect);
				~DiffuseLighting();

				Variable& GetWVP() const { return m_wvp; }
				Variable& GetWorld() const { return m_world; }
				Variable& GetAmbientColor() const { return m_ambientColor; }
				Variable& GetLightColor() const { return m_lightColor; }
				Variable& GetLightDirection() const { return m_lightDirection; }
				Variable& GetColorTexture() const { return m_colorTexture; }

				ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
				unsigned GetVertexSize() const override { return sizeof(Vertex); }

			private:
				void InitializeInternal() override;

				Variable& m_wvp;
				Variable& m_world;
				Variable& m_ambientColor;
				Variable& m_lightColor;
				Variable& m_lightDirection;
				Variable& m_colorTexture;
			};

		} // namespace materials
	} // namespace effect
} // namespace library
