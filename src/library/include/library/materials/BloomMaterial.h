#pragma once
#include "library/materials/PostProcessingMaterial.h"

namespace library
{
	class BloomMaterial : public PostProcessingMaterial
	{
		RTTI_CLASS(BloomMaterial, PostProcessingMaterial)

	public:
		explicit BloomMaterial(const Effect& effect);
		~BloomMaterial();

		EffectVariable& GetBloomTexture() const { return m_bloomTexture; }
		EffectVariable& GetBloomThreshold() const { return m_bloomThreshold; }
		EffectVariable& GetBloomIntensity() const { return m_bloomIntensity; }
		EffectVariable& GetBloomSaturation() const { return m_bloomSaturation; }

		EffectVariable& GetSceneIntensity() const { return m_sceneIntensity; }
		EffectVariable& GetSceneSaturation() const { return m_sceneSaturation; }

	private:
		EffectVariable& m_bloomTexture;
		EffectVariable& m_bloomThreshold;
		EffectVariable& m_bloomIntensity;
		EffectVariable& m_bloomSaturation;

		EffectVariable& m_sceneIntensity;
		EffectVariable& m_sceneSaturation;
	};
} // namespace library
