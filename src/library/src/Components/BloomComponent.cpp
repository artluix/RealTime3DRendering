#include "StdAfx.h"
#include "library/Components/BloomComponent.h"

#include "library/Components/GaussianBlurComponent.h"
#include "library/Components/FullScreenQuadComponent.h"

#include "library/Effect/EffectVariable.h"

#include "library/Application.h"
#include "library/Render/Renderer.h"
#include "library/RenderTargets/FullScreenRenderTarget.h"
#include "library/Math/Color.h"

namespace library
{

namespace
{
const BloomSettings k_defaultBloomSettings = {
	0.45f, // bloomThreshold
	2.0f,  // blurAmount
	1.25f, // bloomSaturation
	1.0f,  // bloomIntensity
	1.0f,  // sceneIntensity
	1.0f,  // sceneSaturation
};

constexpr auto k_backgroundColor = colors::Black;
} // anonymous namespace

std::string BloomDrawModeToString(const BloomDrawMode bloomDrawMode)
{
	switch (bloomDrawMode)
	{
		case BloomDrawMode::Normal:				return "Normal";
		case BloomDrawMode::ExtractedTexture:	return "Extracted Texture";
		case BloomDrawMode::BlurredTexture:		return "Blurred Texture";
		default:								return "";
	}
}

BloomDrawMode BloomDrawModeNext(const BloomDrawMode bloomDrawMode)
{
	switch (bloomDrawMode)
	{
		case BloomDrawMode::Normal:				return BloomDrawMode::ExtractedTexture;
		case BloomDrawMode::ExtractedTexture:	return BloomDrawMode::BlurredTexture;
		case BloomDrawMode::BlurredTexture:		return BloomDrawMode::Normal;
		default:								return BloomDrawMode::Normal; // to skip warning
	}
}

//-------------------------------------------------------------------------

BloomComponent::BloomComponent() : m_settings(k_defaultBloomSettings), m_drawMode(BloomDrawMode::Normal) {}
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

void BloomComponent::InitializeInternal()
{
	CreateMaterialWithEffect("Bloom");
	InitializeQuad();

	m_renderTarget = std::make_unique<FullScreenRenderTarget>(GetApp());

	m_gaussianBlur = std::make_unique<GaussianBlurComponent>();
	m_gaussianBlur->SetSceneTextureSRV(m_renderTarget->GetRenderTargetBuffer()->GetSRV());
	m_gaussianBlur->Initialize(GetApp());
	m_gaussianBlur->SetBlurAmount(m_settings.blurAmount);
}

void BloomComponent::Draw(const Time& time)
{
	switch (m_drawMode)
	{
		case BloomDrawMode::Normal:				DrawNormal(time); break;
		case BloomDrawMode::ExtractedTexture:	DrawExtractedTexture(time); break;
		case BloomDrawMode::BlurredTexture:		DrawBlurredTexture(time); break;
		default: break;
	};
}

//-------------------------------------------------------------------------

void BloomComponent::UpdateExtractMaterial(Material& material) const
{
	material.GetSceneTexture() << GetSceneTextureSRV();
	material.GetBloomThreshold() << m_settings.bloomThreshold;
}

void BloomComponent::UpdateCompositeMaterial(Material& material) const
{
	material.GetSceneTexture() << GetSceneTextureSRV();
	material.GetBloomTexture() << m_gaussianBlurRT->GetRenderTargetBuffer()->GetSRV();
	material.GetBloomIntensity() << m_settings.bloomIntensity;
	material.GetBloomSaturation() << m_settings.bloomSaturation;
	material.GetSceneIntensity() << m_settings.sceneIntensity;
	material.GetSceneSaturation() << m_settings.sceneSaturation;
}

//-------------------------------------------------------------------------

void BloomComponent::DrawNormal(const Time& time)
{
	// Extract bright spots in the scene
	m_renderTarget->Begin();
	m_renderTarget->Clear(k_backgroundColor);

	m_fullScreenQuad->SetActiveTechnique("bloom_extract", "p0");
	m_fullScreenQuad->SetMaterialUpdateFunction(std::bind(&BloomComponent::UpdateExtractMaterial, this, std::ref(*m_material)));
	m_fullScreenQuad->Draw(time);

	m_renderTarget->End();
	GetApp().GetRenderer()->UnbindPSResources(0, 1);

	// Blur the bright spots in the scene
	m_gaussianBlurRT->Begin();
	m_gaussianBlurRT->Clear(k_backgroundColor);
	m_gaussianBlur->Draw(time);
	m_gaussianBlurRT->End();
	GetApp().GetRenderer()->UnbindPSResources(0, 1);

	// Combine the original scene with the blurred bright spot image
	m_fullScreenQuad->SetActiveTechnique("bloom_composite", "p0");
	m_fullScreenQuad->SetMaterialUpdateFunction(std::bind(&BloomComponent::UpdateCompositeMaterial, this, std::ref(*m_material)));
	m_fullScreenQuad->Draw(time);
	GetApp().GetRenderer()->UnbindPSResources(0, 2);
}

void BloomComponent::DrawExtractedTexture(const Time& time)
{
	m_fullScreenQuad->SetActiveTechnique("bloom_extract", "p0");
	m_fullScreenQuad->SetMaterialUpdateFunction(
		std::bind(&BloomComponent::UpdateExtractMaterial, this, std::ref(*m_material)));
	m_fullScreenQuad->Draw(time);
}

void BloomComponent::DrawBlurredTexture(const Time& time)
{
	// Extract bright spots in the scene
	m_renderTarget->Begin();
	m_renderTarget->Clear(k_backgroundColor);

	m_fullScreenQuad->SetActiveTechnique("bloom_extract", "p0");
	m_fullScreenQuad->SetMaterialUpdateFunction(std::bind(&BloomComponent::UpdateExtractMaterial, this, std::ref(*m_material)));
	m_fullScreenQuad->Draw(time);

	m_renderTarget->End();

	GetApp().GetRenderer()->UnbindPSResources(0, 1);

	m_gaussianBlur->Draw(time);
}
} // namespace library
