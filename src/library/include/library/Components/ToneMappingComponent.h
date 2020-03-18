#pragma once
#include "library/Components/ConcreteMaterialPostProcessingComponent.hpp"
#include "library/Materials/ToneMappingMaterial.h"

namespace library
{
class SingleRenderTarget;

class ToneMappingComponent : public ConcreteMaterialPostProcessingComponent<ToneMappingMaterial>
{
	RTTI_CLASS(ToneMappingComponent, PostProcessingComponent)

public:
	ToneMappingComponent();
	~ToneMappingComponent();

	bool IsGammaCorrectionEnabled() const { return m_gammaCorrectionEnabled; }
	void SetGammaCorrection(const bool enabled);

	unsigned GetLuminanceSamplingStepsCount() const { return m_luminanceSamplingStepsCount; }
	void SetLuminanceSamplingStepsCount(const unsigned stepsCount);

	unsigned GetLuminanceSamplingMipsCount() const { return m_luminanceSamplingMipsCount; }

	void Draw(const Time& time);

protected:
	void InitializeInternal() override;

private:
	void UpdateLuminanceMaterial(Material& material);
	void UpdateTonemapMaterial(Material& material);

	std::unique_ptr<SingleRenderTarget> m_luminanceLogRT;
	unsigned m_luminanceSamplingStepsCount;
	unsigned m_luminanceSamplingMipsCount;

	bool m_gammaCorrectionEnabled;
};
} // namespace library
