#pragma once
#include "Materials/PointLightMaterial.h"

class SkinnedModelMaterial : public PointLightMaterial
{
	RTTI_CLASS(SkinnedModelMaterial, PointLightMaterial)

public:
	using Vertex = library::VertexPositionTextureNormalSkinned;

	explicit SkinnedModelMaterial(library::Effect& effect);
	~SkinnedModelMaterial();

	library::EffectVariable& GetBoneTransforms() { return m_boneTransforms; }

	library::VertexBufferData CreateVertexBufferData(
		ID3D11Device* const device,
		const library::Mesh& mesh
	) const override;
	unsigned GetVertexSize() const override { return sizeof(Vertex); }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_boneTransforms;
};
