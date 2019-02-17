#pragma once
#include "library/materials/BloomMaterial.h"
#include "library/components/ConcretePostProcessingComponent.hpp"

namespace library
{
	struct BloomSettings
	{
		float bloomThreshold;
		float bloomIntensity;
		float bloomSaturation;
		float blurAmount;
		float sceneIntensity;
		float sceneSaturation;
	};

	//-------------------------------------------------------------------------

	enum class BloomDrawMode
	{
		None = -1,
		ExtractedTexture = 0,
		BlurredTexture,
	};

	std::string BloomDrawModeToString(const BloomDrawMode bloomDrawMode);

	//-------------------------------------------------------------------------

	class GaussianBlurComponent;

	class BloomComponent : public ConcretePostProcessingComponent<BloomMaterial>
	{
		RTTI_CLASS(BloomComponent, PostProcessingComponent)

	public:
		explicit BloomComponent();
		~BloomComponent();

		const BloomSettings& GetSettings() const { return m_settings; }
		void SetSettings(const BloomSettings& settings);

		BloomDrawMode GetDrawMode() const { return m_drawMode; }
		void SetDrawMode(const BloomDrawMode drawMode);

		void Initialize(const Application& app) override;
		void Draw(const Time& time) override;

	private:
		void UpdateExtractMaterial();
		void UpdateCompositeMaterial();

		void DrawExtractedTexture(const Time& time);
		void DrawBlurredTexture(const Time& time);

		BloomSettings m_settings;
		BloomDrawMode m_drawMode;

		std::unique_ptr<FullScreenRenderTarget> m_renderTarget;
		std::unique_ptr<GaussianBlurComponent> m_gaussianBlur;
	};
} // namespace library
