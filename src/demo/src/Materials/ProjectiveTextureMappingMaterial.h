#pragma once
#include "Materials/PhongLightingMaterial.h"

class ProjectiveTextureMappingMaterial : public PhongLightingMaterial
{
	RTTI_CLASS(ProjectiveTextureMappingMaterial, PhongLightingMaterial)

public:
	using Vertex = library::VertexPositionTextureNormal;

	explicit ProjectiveTextureMappingMaterial(library::Effect& effect);

	library::EffectVariable& GetProjectedTexture() { return m_projectedTexture; }
	library::EffectVariable& GetProjectiveTextureMatrix() { return m_projectiveTextureMatrix; }

	library::EffectVariable& GetDepthBias() { return m_depthBias; }
	library::EffectVariable& GetDepthMapTexture() { return m_depthMapTextureMatrix; }

	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }
	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }
	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_projectedTexture;
	library::EffectVariable& m_projectiveTextureMatrix;

	library::EffectVariable& m_depthBias;
	library::EffectVariable& m_depthMapTextureMatrix;

	library::EffectVariable& m_cameraPosition;
	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;
	library::EffectVariable& m_colorTexture;
};
