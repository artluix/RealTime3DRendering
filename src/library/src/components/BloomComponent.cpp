#include "StdAfx.h"
#include "library/components/BloomComponent.h"

#include "library/components/GaussianBlurComponent.h"

#include "library/FullScreenRenderTarget.h"

namespace library
{
	namespace
	{
		const BloomSettings k_defaultBloomSettings =
		{
			0.45f,	// bloomThreshold
			2.0f,	// bloomIntensity
			1.25f,	// bloomSaturation
			1.0f,	// blurAmount
			1.0f,	// sceneIntensity
			1.0f,	// sceneSaturation
		};
	}

	std::string BloomDrawModeToString(const BloomDrawMode bloomDrawMode)
	{
		switch (bloomDrawMode)
		{
			case BloomDrawMode::None:				return "None";
			case BloomDrawMode::ExtractedTexture:	return "Extracted Texture";
			case BloomDrawMode::BlurredTexture:		return "Blurred Texture";
			default:									return "";
		}
	}

	//-------------------------------------------------------------------------

	BloomComponent::BloomComponent()
		: m_settings(k_defaultBloomSettings)
		, m_drawMode(BloomDrawMode::None)
	{
	}

	BloomComponent::~BloomComponent() = default;

	//-------------------------------------------------------------------------

	void BloomComponent::SetSettings(const BloomSettings& settings)
	{
		m_settings = settings;
		m_gaussianBlur->SetBlurAmount(m_settings.blurAmount);
	}

	void BloomComponent::SetDrawMode(const BloomDrawMode drawMode)
	{
		m_drawMode = drawMode;
	}

	//-------------------------------------------------------------------------

	void BloomComponent::Initialize(const Application& app)
	{
		PostProcessingComponent::Initialize(app);
		
		InitializeMaterial(app, "Bloom");
	}

	void BloomComponent::Draw(const Time& time)
	{
		switch (m_drawMode)
		{
			case BloomDrawMode::ExtractedTexture:
				DrawExtractedTexture(time);
				break;

			case BloomDrawMode::BlurredTexture:
				DrawBlurredTexture(time);
				break;

			default:
				break;
		};
	}

	//-------------------------------------------------------------------------

	void BloomComponent::UpdateExtractMaterial()
	{
		
	}

	void BloomComponent::UpdateCompositeMaterial()
	{

	}

	//-------------------------------------------------------------------------

	void BloomComponent::DrawExtractedTexture(const Time& time)
	{

	}

	void BloomComponent::DrawBlurredTexture(const Time& time)
	{

	}
} // namespace library
