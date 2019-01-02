#pragma once
#include <library/RTTI.hpp>
#include <library/effects/EffectMaterial.h>

#include <DirectXMath.h>

namespace demo
{
	class BasicMaterial : public library::rtti::Class<BasicMaterial, library::EffectMaterial>
	{
	public:
		struct Vertex
		{
			DirectX::XMFLOAT4 position;
			DirectX::XMFLOAT4 color;

			explicit Vertex(const DirectX::XMFLOAT4& position, const DirectX::XMFLOAT4& color)
				: position(position)
				, color(color)
			{
			}
		};

		using Vertices = std::vector<Vertex>;

		//-------------------------------------------------------------------------

		explicit BasicMaterial();

		library::EffectVariable& GetWorldViewProjection() const { return *m_worldViewProjection; }

		void Initialize(library::Effect* const effect) override;

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const library::Mesh& mesh) const override;
		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Vertices& vertices) const;

		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		library::EffectVariable* m_worldViewProjection;
	};

} // namespace demo
