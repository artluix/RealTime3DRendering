#pragma once
#include "Materials/PhongLightingMaterial.h"

class SkinnedModelMaterial : public PhongLightingMaterial
{
	RTTI_CLASS(SkinnedModelMaterial, PhongLightingMaterial)

public:
	using Vertex = library::VertexPositionTextureNormalSkinned;

	explicit SkinnedModelMaterial(library::Effect& effect);
	~SkinnedModelMaterial();

	library::EffectVariable& GetBoneTransforms() { return m_boneTransforms; }

	library::VertexBufferData CreateVertexBufferData(
		ID3D11Device* const device,
		const library::Mesh& mesh
	) const override;

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_boneTransforms;
};
