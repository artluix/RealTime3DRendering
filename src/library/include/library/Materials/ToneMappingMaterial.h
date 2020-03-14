#pragma once
#include "library/Materials/PostProcessingMaterial.h"

namespace library
{
class ToneMappingMaterial : public PostProcessingMaterial
{
	RTTI_CLASS(ToneMappingMaterial, PostProcessingMaterial)

public:
	explicit ToneMappingMaterial(Effect& effect);
	~ToneMappingMaterial();

	EffectVariable& GetExposure() { return m_exposure; }

private:
	EffectVariable& m_exposure;
};
} // namespace library
