#include "StdAfx.h"
#include "library/Components/ToneMappingComponent.h"

#include "library/Components/FullScreenQuadComponent.h"
#include "library/RenderTargets/SingleRenderTarget.h"
#include "library/Render/Renderer.h"

#include "library/Effect/EffectVariable.h"

#include "library/Application.h"
#include "library/Utils.h"

namespace library
{
ToneMappingComponent::ToneMappingComponent()
	: m_gammaCorrectionEnabled(false)
	, m_luminanceSamplingStepsCount(0)
	, m_luminanceSamplingMipsCount(1)
{
}

ToneMappingComponent::~ToneMappingComponent() = default;

//-------------------------------------------------------------------------

void ToneMappingComponent::InitializeInternal()
{
	CreateMaterialWithEffect("ToneMapping");
	InitializeQuad();

	// setup luminosity log target
	const auto screenWidth = GetApp().GetScreenWidth();
	const auto screenHeight = GetApp().GetScreenHeight();

	m_luminanceSamplingMipsCount = utils::GetMipLevelsCount(screenWidth, screenHeight);

	{
		// setup render target buffer
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = screenWidth;
		textureDesc.Height = screenHeight;
		textureDesc.MipLevels = m_luminanceSamplingMipsCount;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32_FLOAT; // need only 1 float value
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		auto rtBuffer = RenderTargetBuffer::Create(GetApp().GetDevice(), textureDesc);

		m_luminanceLogRT = std::make_unique<SingleRenderTarget>(GetApp(), std::move(rtBuffer));
	}
}

//-------------------------------------------------------------------------

void ToneMappingComponent::UpdateLuminanceMaterial(Material& material)
{
	material.GetSceneTexture() << GetSceneTextureSRV();
}

void ToneMappingComponent::UpdateTonemapMaterial(Material& material)
{
	material.GetSceneTexture() << GetSceneTextureSRV();
	material.GetLuminanceLogTexture() << m_luminanceLogRT->GetRenderTargetBuffer()->GetSRV();
	material.GetLuminanceSamplingStepsCount() << m_luminanceSamplingStepsCount;
	material.GetLuminanceSamplingMipsCount() << m_luminanceSamplingStepsCount;
}

//-------------------------------------------------------------------------

void ToneMappingComponent::SetGammaCorrection(const bool enabled)
{
	m_gammaCorrectionEnabled = enabled;
}

void ToneMappingComponent::SetLuminanceSamplingStepsCount(const unsigned stepsCount)
{
	assert(stepsCount < m_luminanceSamplingMipsCount);
	m_luminanceSamplingStepsCount = stepsCount;
}

//-------------------------------------------------------------------------

void ToneMappingComponent::Draw(const Time& time)
{
	m_luminanceLogRT->Begin();
	m_luminanceLogRT->Clear();

	m_fullScreenQuad->SetActiveTechnique("calc_luminance_log");
	m_fullScreenQuad->SetMaterialUpdateFunction(std::bind(&ToneMappingComponent::UpdateLuminanceMaterial, this, std::ref(*m_material)));
	m_fullScreenQuad->Draw(time);

	m_luminanceLogRT->End();
	GetApp().GetRenderer()->UnbindPSResources(0, 1);

	auto deviceContext = GetApp().GetDeviceContext();
	deviceContext->GenerateMips(m_luminanceLogRT->GetRenderTargetBuffer()->GetSRV());

	m_fullScreenQuad->SetActiveTechnique(m_gammaCorrectionEnabled ? "tone_mapping_gamma" : "tone_mapping");
	m_fullScreenQuad->SetMaterialUpdateFunction(std::bind(&ToneMappingComponent::UpdateTonemapMaterial, this, std::ref(*m_material)));

	m_fullScreenQuad->Draw(time);
	GetApp().GetRenderer()->UnbindPSResources(0, 2);
}
} // namespace library
