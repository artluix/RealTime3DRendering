#include "Materials/DistortionMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

DistortionMappingMaterial::DistortionMappingMaterial(Effect& effect)
	: PostProcessingMaterial(effect)
	, m_distortionMapTexture(effect.GetVariable("DistortionMapTexture"))
	, m_displacementScale(effect.GetVariable("displacementScale"))
	, m_wvp(effect.GetVariable("wvp"))
{}

DistortionMappingMaterial::~DistortionMappingMaterial() = default;
