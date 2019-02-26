#pragma once
#include "library/materials/BloomMaterial.h"
#include "library/components/ConcreteMaterialPostProcessingComponent.hpp"

namespace library
{
	struct BloomSettings
	{
		float bloomThreshold;
		float blurAmount;
		float bloomIntensity;
		float bloomSaturation;
		float sceneIntensity;
		float sceneSaturation;
	};

	//-------------------------------------------------------------------------

	enum class BloomDrawMode
	{
		Normal = 0,
		ExtractedTexture,
		BlurredTexture,
	};

	std::string BloomDrawModeToString(const BloomDrawMode bloomDrawMode);
	BloomDrawMode BloomDrawModeNext(const BloomDrawMode bloomDrawMode);

	//-------------------------------------------------------------------------

	class GaussianBlurComponent;

	class BloomComponent : public ConcreteMaterialPostProcessingComponent<BloomMaterial>
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

	protected:
		BloomDrawMode m_drawMode;

	private:
		void UpdateExtractMaterial(Material& material) const;
		void UpdateCompositeMaterial(Material& material) const;

		void DrawNormal(const Time& time);
		void DrawExtractedTexture(const Time& time);
		void DrawBlurredTexture(const Time& time);

		BloomSettings m_settings;

		std::unique_ptr<FullScreenRenderTarget> m_renderTarget;
		std::unique_ptr<GaussianBlurComponent> m_gaussianBlur;
	};
} // namespace library
