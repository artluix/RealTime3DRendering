#pragma once
#include "library/materials/PointLightMaterial.h"

namespace library
{
	class ProjectiveTextureMappingMaterial : public PointLightMaterial
	{
		RTTI_CLASS(ProjectiveTextureMappingMaterial, Material)

	public:
		explicit ProjectiveTextureMappingMaterial(Effect& effect);

		EffectVariable& GetProjectedTexture() { return m_projectedTexture; }
		EffectVariable& GetProjectiveTextureMatrix() { return m_projectiveTextureMatrix; }

	private:
		void InitializeInternal() override;

		EffectVariable& m_projectedTexture;
		EffectVariable& m_projectiveTextureMatrix;
	};
} // namespace library
