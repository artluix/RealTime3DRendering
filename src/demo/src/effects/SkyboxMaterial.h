#pragma once
#include <library/RTTI.hpp>
#include <library/effects/EffectMaterial.h>

#include <DirectXMath.h>

namespace demo
{
	class SkyboxMaterial : public library::rtti::Class<SkyboxMaterial, library::EffectMaterial>
	{
	public:
		struct Vertex
		{
			DirectX::XMFLOAT4 position;

			explicit Vertex(const DirectX::XMFLOAT4& position)
				: position(position)
			{
			}
		};

		using Vertices = std::vector<Vertex>;

		//-------------------------------------------------------------------------

		explicit SkyboxMaterial(library::Effect& effect);

		library::EffectVariable& GetWorldViewProjection() const { return *m_worldViewProjection; }
		library::EffectVariable& GetSkyboxTexture() const { return *m_skyboxTexture; }

		void Initialize() override;

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const library::Mesh& mesh) const override;
		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Vertices& vertices) const;

		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	private:
		library::EffectVariable* m_worldViewProjection;
		library::EffectVariable* m_skyboxTexture;
	};

} // namespace demo
