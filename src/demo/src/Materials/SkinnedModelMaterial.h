#pragma once
#include "Materials/LightMaterial.h"

class SkinnedModelMaterial : public LightMaterial
{
	RTTI_CLASS(SkinnedModelMaterial, LightMaterial)

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
