#pragma once
#include "library/RTTI.hpp"
#include "library/effect/EffectMaterial.h"
#include "library/VertexTypes.h"

namespace library
{
	class DiffuseLightingMaterial : public library::rtti::Class<DiffuseLightingMaterial, EffectMaterial>
	{
	public:
		using Vertex = VertexPositionTextureNormal;
		using Vertices = std::vector<Vertex>;

		explicit DiffuseLightingMaterial(const library::Effect& effect);
		~DiffuseLightingMaterial();

		EffectVariable& GetWVP() const { return m_wvp; }
		EffectVariable& GetWorld() const { return m_world; }
		EffectVariable& GetAmbientColor() const { return m_ambientColor; }
		EffectVariable& GetLightColor() const { return m_lightColor; }
		EffectVariable& GetLightDirection() const { return m_lightDirection; }
		EffectVariable& GetColorTexture() const { return m_colorTexture; }

		void Initialize() override;

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const library::Mesh& mesh) const override;
		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Vertices& vertices) const;

		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		EffectVariable& m_wvp;
		EffectVariable& m_world;
		EffectVariable& m_ambientColor;
		EffectVariable& m_lightColor;
		EffectVariable& m_lightDirection;
		EffectVariable& m_colorTexture;
	};

} // namespace library
