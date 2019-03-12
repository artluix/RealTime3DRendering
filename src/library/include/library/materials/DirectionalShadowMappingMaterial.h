#pragma once
#include "library/Materials/DirectionalLightMaterial.h"

namespace library
{
	class DirectionalShadowMappingMaterial : public DirectionalLightMaterial
	{
		RTTI_CLASS(DirectionalShadowMappingMaterial, DirectionalLightMaterial)

	public:
		explicit DirectionalShadowMappingMaterial(Effect& effect);

		EffectVariable& GetProjectiveTextureMatrix() { return m_projectiveTextureMatrix; }
		EffectVariable& GetShadowMapTexture() { return m_shadowMapTexture; }
		EffectVariable& GetShadowMapSize() { return m_shadowMapSize; }

	private:
		void InitializeInternal() override;

		EffectVariable& m_projectiveTextureMatrix;
		EffectVariable& m_shadowMapTexture;
		EffectVariable& m_shadowMapSize;
	};
} // namespace library
