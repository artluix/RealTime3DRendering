#pragma once
#include <library/Materials/PostProcessingMaterial.h>

class ToneMappingMaterial : public library::PostProcessingMaterial
{
	RTTI_CLASS(ToneMappingMaterial, library::PostProcessingMaterial)

public:
	explicit ToneMappingMaterial(library::Effect& effect);
	~ToneMappingMaterial();

	library::EffectVariable& GetExposure() { return m_exposure; }

private:
	library::EffectVariable& m_exposure;
};
