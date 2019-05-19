#pragma once
#include "library/Materials/PointLightMaterial.h"

namespace library
{
class SkinnedModelMaterial : public PointLightMaterial
{
	RTTI_CLASS(SkinnedModelMaterial, PointLightMaterial)

public:
	using Vertex = VertexPositionTextureNormalSkinned;

	explicit SkinnedModelMaterial(std::shared_ptr<Effect> effect);
	~SkinnedModelMaterial();

	EffectVariable& GetBoneTransforms() { return m_boneTransforms; }

	VertexBufferData CreateVertexBufferData(ID3D11Device* const device, const Mesh& mesh) const override;
	unsigned GetVertexSize() const override { return sizeof(Vertex); }

protected:
	void InitializeInternal() override;

private:
	EffectVariable& m_boneTransforms;
};
} // namespace library
