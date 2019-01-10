#pragma once
#include "library/effect/EffectMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	namespace effect
	{
		namespace materials
		{
			class Skybox : public Material
			{
				RTTI_CLASS(Skybox, Material)

			public:
				using Vertex = VertexPosition;

				explicit Skybox(const Effect& effect);
				~Skybox();

				Variable& GetWorldViewProjection() const { return m_worldViewProjection; }
				Variable& GetSkyboxTexture() const { return m_skyboxTexture; }

				ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
				unsigned GetVertexSize() const override { return sizeof(Vertex); }

			private:
				void InitializeInternal() override;

				Variable& m_worldViewProjection;
				Variable& m_skyboxTexture;
			};

		} // namespace materials
	} // namespace effect
} // namespace library
