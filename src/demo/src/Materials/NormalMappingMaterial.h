#pragma once
#include "Materials/PhongLightingMaterial.h"

class NormalMappingMaterial : public PhongLightingMaterial
{
	RTTI_CLASS(NormalMappingMaterial, PhongLightingMaterial)

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
