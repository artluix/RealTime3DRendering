#pragma once
#include "Materials/LightsMaterial.h"

class NormalMappingMaterial : public LightsMaterial
{
	RTTI_CLASS(NormalMappingMaterial, LightsMaterial)

public:
	using Vertex = library::VertexPositionTextureNormalTangent;

	explicit NormalMappingMaterial(library::Effect& effect);
	~NormalMappingMaterial();

	library::EffectVariable& GetNormalMap() { return m_normalMap; }

	library::VertexBufferData CreateVertexBufferData(
		ID3D11Device* const device,
		const library::Mesh& mesh
	) const override;

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_normalMap;
};
