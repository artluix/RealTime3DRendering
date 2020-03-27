#pragma once
#include "Materials/PhongLightingMaterial.h"

class ProjectiveTextureMappingMaterial : public PhongLightingMaterial
{
	RTTI_CLASS(ProjectiveTextureMappingMaterial, PhongLightingMaterial)

public:
	explicit ProjectiveTextureMappingMaterial(library::Effect& effect);

	library::EffectVariable& GetProjectedTexture() { return m_projectedTexture; }
	library::EffectVariable& GetProjectiveTextureMatrix() { return m_projectiveTextureMatrix; }

	library::EffectVariable& GetDepthBias() { return m_depthBias; }
	library::EffectVariable& GetDepthMapTexture() { return m_depthMapTextureMatrix; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_projectedTexture;
	library::EffectVariable& m_projectiveTextureMatrix;

	library::EffectVariable& m_depthBias;
	library::EffectVariable& m_depthMapTextureMatrix;
};
