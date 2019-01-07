#pragma once
#include "library/materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
	class SkyboxMaterial : public Material
	{
		RTTI_CLASS(SkyboxMaterial, Material)

	public:
		using Vertex = VertexPosition;

		explicit SkyboxMaterial(const Effect& effect);
		~SkyboxMaterial();

		EffectVariable& GetWorldViewProjection() const { return m_worldViewProjection; }
		EffectVariable& GetSkyboxTexture() const { return m_skyboxTexture; }

		void Initialize() override;

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		EffectVariable& m_worldViewProjection;
		EffectVariable& m_skyboxTexture;
	};

} // namespace library
