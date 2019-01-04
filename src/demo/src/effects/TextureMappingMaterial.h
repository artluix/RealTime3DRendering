#pragma once
#include <library/RTTI.hpp>
#include <library/effects/EffectMaterial.h>
#include <library/VertexTypes.h>

namespace demo
{
	class TextureMappingMaterial : public library::rtti::Class<TextureMappingMaterial, library::EffectMaterial>
	{
	public:
		using Vertex = library::VertexPositionTexture;
		using Vertices = std::vector<Vertex>;

		explicit TextureMappingMaterial(const library::Effect& effect);
		~TextureMappingMaterial();

		library::EffectVariable& GetWorldViewProjection() const { return m_worldViewProjection; }
		library::EffectVariable& GetColorTexture() const { return m_colorTexture; }

		void Initialize() override;

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const library::Mesh& mesh) const override;
		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Vertices& vertices) const;

		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		library::EffectVariable& m_worldViewProjection;
		library::EffectVariable& m_colorTexture;
	};

} // namespace demo
