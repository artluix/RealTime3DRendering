#pragma once
#include "library/Materials/Material.h"
#include "library/VertexTypes.h"

namespace library
{
	class DepthMapMaterial : public Material
	{
		RTTI_CLASS(DepthMapMaterial, Material)

	public:
		using Vertex = VertexPosition;

		explicit DepthMapMaterial(Effect& effect);
		~DepthMapMaterial();

		EffectVariable& GetWorldLightViewProjection() { return m_worldLightViewProjection; }

		ComPtr<ID3D11Buffer> CreateVertexBuffer(ID3D11Device* const device, const Mesh& mesh) const override;
		unsigned GetVertexSize() const override { return sizeof(Vertex); }

	protected:
		void InitializeInternal() override;

	private:
		EffectVariable& m_worldLightViewProjection;
	};
} // namespace library
