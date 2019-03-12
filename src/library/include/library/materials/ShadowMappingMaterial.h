#pragma once
#include "library/Materials/PointLightMaterial.h"

namespace library
{
	class ShadowMappingMaterial : public PointLightMaterial
	{
		RTTI_CLASS(ShadowMappingMaterial, PointLightMaterial)

	public:
		explicit ShadowMappingMaterial(Effect& effect);

		EffectVariable& GetProjectiveTextureMatrix() { return m_projectiveTextureMatrix; }
		EffectVariable& GetShadowMapTexture() { return m_shadowMapTexture; }
		EffectVariable& GetShadowMapSize() { return m_shadowMapSize; }

	protected:
		void InitializeInternal() override;

	private:
		EffectVariable& m_projectiveTextureMatrix;
		EffectVariable& m_shadowMapTexture;
		EffectVariable& m_shadowMapSize;
	};
} // namespace library
