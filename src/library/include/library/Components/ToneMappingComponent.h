#pragma once
#include "library/Components/ConcreteMaterialPostProcessingComponent.hpp"
#include "library/Materials/ToneMappingMaterial.h"

namespace library
{
class ToneMappingComponent : public ConcreteMaterialPostProcessingComponent<ToneMappingMaterial>
{
	RTTI_CLASS(ToneMappingComponent, PostProcessingComponent)

public:
	ToneMappingComponent();
	~ToneMappingComponent();

	float GetExposure() const { return m_exposure; }
	void SetExposure(const float exposure);

	bool IsGammaCorrectionEnabled() const { return m_gammaCorrectionEnabled; }
	void SetGammaCorrection(const bool enabled);

private:
	float m_exposure;
	bool m_gammaCorrectionEnabled;
};
} // namespace library
