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

	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }
	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }
	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_boneTransforms;

	library::EffectVariable& m_cameraPosition;
	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;
	library::EffectVariable& m_colorTexture;
};
