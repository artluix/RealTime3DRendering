#pragma once
#include <library/RTTI.hpp>
#include <library/effects/EffectMaterial.h>
#include <library/VertexTypes.h>

namespace demo
{
	class BasicMaterial : public library::rtti::Class<BasicMaterial, library::EffectMaterial>
	{
	public:
		using Vertex = library::VertexPositionColor;
		using Vertices = std::vector<Vertex>;

		explicit BasicMaterial(const library::Effect& effect);
		~BasicMaterial();

		library::EffectVariable& GetWorldViewProjection() const { return m_worldViewProjection; }

		void Initialize() override;

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const library::Mesh& mesh) const override;
		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Vertices& vertices) const;

		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		library::EffectVariable& m_worldViewProjection;
	};

} // namespace demo
