#include "StdAfx.h"
#include "library/components/BloomComponent.h"

#include "library/components/GaussianBlurComponent.h"

#include "library/effect/EffectVariable.h"

#include "library/Application.h"
#include "library/FullScreenRenderTarget.h"
#include "library/Color.h"

namespace library
{
	namespace
	{
		const BloomSettings k_defaultBloomSettings =
		{
			0.45f,	// bloomThreshold
			2.0f,	// blurAmount
			1.25f,	// bloomSaturation
			1.0f,	// bloomIntensity
			1.0f,	// sceneIntensity
			1.0f,	// sceneSaturation
		};

		const auto k_backgroundColor = library::Color::Black;
	}

	std::string BloomDrawModeToString(const BloomDrawMode bloomDrawMode)
	{
		switch (bloomDrawMode)
		{
			case BloomDrawMode::Normal:				return "Normal";
			case BloomDrawMode::ExtractedTexture:	return "Extracted Texture";
			case BloomDrawMode::BlurredTexture:		return "Blurred Texture";
			default:									return "";
		}
	}

	BloomDrawMode BloomDrawModeNext(const BloomDrawMode bloomDrawMode)
	{
		switch (bloomDrawMode)
		{
			case BloomDrawMode::Normal:				return BloomDrawMode::ExtractedTexture;
			case BloomDrawMode::ExtractedTexture:	return BloomDrawMode::BlurredTexture;
			case BloomDrawMode::BlurredTexture:		return BloomDrawMode::Normal;
			default:									return BloomDrawMode::Normal; // to skip warning
		}
	}

	//-------------------------------------------------------------------------

	BloomComponent::BloomComponent()
		: m_settings(k_defaultBloomSettings)
		, m_drawMode(BloomDrawMode::Normal)
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
		InitializeQuad(app);

		m_renderTarget = std::make_unique<FullScreenRenderTarget>(app);

		m_gaussianBlur = std::make_unique<GaussianBlurComponent>();
		m_gaussianBlur->SetSceneTexture(*m_renderTarget->GetOutputTexture());
		m_gaussianBlur->Initialize(app);
		m_gaussianBlur->SetBlurAmount(m_settings.blurAmount);
	}

	void BloomComponent::Draw(const Time& time)
	{
		switch (m_drawMode)
		{
			case BloomDrawMode::Normal:
				DrawNormal(time);
				break;

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
		m_material->GetColorTexture() << GetSceneTexture();
		m_material->GetBloomThreshold() << m_settings.bloomThreshold;
	}

	void BloomComponent::UpdateCompositeMaterial()
	{
		m_material->GetColorTexture() << GetSceneTexture();
		m_material->GetBloomTexture() << m_gaussianBlur->GetOutputTexture();
		m_material->GetBloomIntensity() << m_settings.bloomIntensity;
		m_material->GetBloomSaturation() << m_settings.bloomSaturation;
		m_material->GetSceneIntensity() << m_settings.sceneIntensity;
		m_material->GetSceneSaturation() << m_settings.sceneSaturation;
	}

	//-------------------------------------------------------------------------
	
	void BloomComponent::DrawNormal(const Time& time)
	{
		auto deviceContext = m_app->GetDeviceContext();

		// Extract bright spots in the scene
		m_renderTarget->Begin();

		deviceContext->ClearRenderTargetView(m_renderTarget->GetRenderTargetView(), static_cast<const float*>(k_backgroundColor));
		deviceContext->ClearDepthStencilView(m_renderTarget->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		m_fullScreenQuad->SetMaterial(*m_material, "bloom_extract", "p0");
		m_fullScreenQuad->SetMaterialUpdateFunction(std::bind(&BloomComponent::UpdateExtractMaterial, this));
		m_fullScreenQuad->Draw(time);

		m_renderTarget->End();
		m_app->UnbindPixelShaderResources(0, 1);

		// Blur the bright spots in the scene
		m_gaussianBlur->DrawToTexture(time);
		m_app->UnbindPixelShaderResources(0, 1);

		// Combine the original scene with the blurred bright spot image
		m_fullScreenQuad->SetMaterial(*m_material, "bloom_composite", "p0");
		m_fullScreenQuad->SetMaterialUpdateFunction(std::bind(&BloomComponent::UpdateCompositeMaterial, this));
		m_fullScreenQuad->Draw(time);
		m_app->UnbindPixelShaderResources(0, 1);
	}

	void BloomComponent::DrawExtractedTexture(const Time& time)
	{
		m_fullScreenQuad->SetMaterial(*m_material, "bloom_extract", "p0");
		m_fullScreenQuad->SetMaterialUpdateFunction(std::bind(&BloomComponent::UpdateExtractMaterial, this));
		m_fullScreenQuad->Draw(time);
	}

	void BloomComponent::DrawBlurredTexture(const Time& time)
	{
		auto deviceContext = m_app->GetDeviceContext();

		// Extract bright spots in the scene
		m_renderTarget->Begin();
		
		deviceContext->ClearRenderTargetView(m_renderTarget->GetRenderTargetView(), static_cast<const float*>(k_backgroundColor));
		deviceContext->ClearDepthStencilView(m_renderTarget->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		
		m_fullScreenQuad->SetMaterial(*m_material, "bloom_extract", "p0");
		m_fullScreenQuad->SetMaterialUpdateFunction(std::bind(&BloomComponent::UpdateExtractMaterial, this));
		m_fullScreenQuad->Draw(time);

		m_renderTarget->End();

		m_app->UnbindPixelShaderResources(0, 1);

		m_gaussianBlur->Draw(time);
	}
} // namespace library
