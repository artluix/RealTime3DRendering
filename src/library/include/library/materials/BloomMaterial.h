#pragma once
#include "library/Materials/PostProcessingMaterial.h"

namespace library
{
class BloomMaterial : public PostProcessingMaterial
{
	RTTI_CLASS(BloomMaterial, PostProcessingMaterial)

public:
	explicit BloomMaterial(Effect& effect);
	~BloomMaterial();

	EffectVariable& GetBloomTexture() { return m_bloomTexture; }
	EffectVariable& GetBloomThreshold() { return m_bloomThreshold; }
	EffectVariable& GetBloomIntensity() { return m_bloomIntensity; }
	EffectVariable& GetBloomSaturation() { return m_bloomSaturation; }

	EffectVariable& GetSceneIntensity() { return m_sceneIntensity; }
	EffectVariable& GetSceneSaturation() { return m_sceneSaturation; }

private:
	EffectVariable& m_bloomTexture;
	EffectVariable& m_bloomThreshold;
	EffectVariable& m_bloomIntensity;
	EffectVariable& m_bloomSaturation;

	EffectVariable& m_sceneIntensity;
	EffectVariable& m_sceneSaturation;
};
} // namespace library
