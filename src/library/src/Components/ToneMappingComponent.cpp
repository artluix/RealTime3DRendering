#include "StdAfx.h"
#include "library/Components/ToneMappingComponent.h"

#include "library/Components/FullScreenQuadComponent.h"

namespace library
{
ToneMappingComponent::ToneMappingComponent()
{
}

ToneMappingComponent::~ToneMappingComponent() = default;

//-------------------------------------------------------------------------

void ToneMappingComponent::SetExposure(const float exposure)
{
	m_exposure = exposure;
}

void ToneMappingComponent::SetGammaCorrection(const bool enabled)
{
	m_gammaCorrectionEnabled = enabled;
}


} // namespace library
