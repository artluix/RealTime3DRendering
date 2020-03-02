#pragma once
#include "Materials/LightsMaterial.h"

class SkinnedModelMaterial : public LightsMaterial
{
	RTTI_CLASS(SkinnedModelMaterial, LightsMaterial)

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
