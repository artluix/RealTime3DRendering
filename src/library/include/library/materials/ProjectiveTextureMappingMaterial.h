#pragma once
#include "library/Materials/PointLightMaterial.h"

namespace library
{
class ProjectiveTextureMappingMaterial : public PointLightMaterial
{
	RTTI_CLASS(ProjectiveTextureMappingMaterial, PointLightMaterial)

public:
	explicit ProjectiveTextureMappingMaterial(std::shared_ptr<Effect> effect);

	EffectVariable& GetProjectedTexture() { return m_projectedTexture; }
	EffectVariable& GetProjectiveTextureMatrix() { return m_projectiveTextureMatrix; }

	EffectVariable& GetDepthBias() { return m_depthBias; }
	EffectVariable& GetDepthMapTexture() { return m_depthMapTextureMatrix; }

protected:
	void InitializeInternal() override;

private:
	EffectVariable& m_projectedTexture;
	EffectVariable& m_projectiveTextureMatrix;

	EffectVariable& m_depthBias;
	EffectVariable& m_depthMapTextureMatrix;
};
} // namespace library
