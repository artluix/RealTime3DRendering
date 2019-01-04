#pragma once
#include "library/RTTI.hpp"
#include "library/effects/EffectMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	class SkyboxMaterial : public library::rtti::Class<SkyboxMaterial, EffectMaterial>
	{
	public:
		using Vertex = VertexPosition;
		using Vertices = std::vector<Vertex>;

		explicit SkyboxMaterial(const library::Effect& effect);
		~SkyboxMaterial();

		EffectVariable& GetWorldViewProjection() const { return m_worldViewProjection; }
		EffectVariable& GetSkyboxTexture() const { return m_skyboxTexture; }

		void Initialize() override;

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const library::Mesh& mesh) const override;
		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Vertices& vertices) const;

		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		EffectVariable& m_worldViewProjection;
		EffectVariable& m_skyboxTexture;
	};

} // namespace library
