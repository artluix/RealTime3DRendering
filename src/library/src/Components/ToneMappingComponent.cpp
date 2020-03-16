#include "StdAfx.h"
#include "library/Components/ToneMappingComponent.h"

#include "library/Components/FullScreenQuadComponent.h"
#include "library/RenderTargets/SingleRenderTarget.h"

#include "library/Application.h"
#include "library/Utils.h"

namespace library
{
ToneMappingComponent::ToneMappingComponent()
{
}

ToneMappingComponent::~ToneMappingComponent() = default;

//-------------------------------------------------------------------------

void ToneMappingComponent::InitializeInternal()
{
	// setup luminosity target
	{
		// setup render target buffer
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = GetApp().GetScreenWidth();
		textureDesc.Height = GetApp().GetScreenHeight();
		textureDesc.MipLevels = utils::GetMipLevelsCount(textureDesc.Width, textureDesc.Height);
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R32_FLOAT; // need only 1 float value
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		auto rtBuffer = RenderTargetBuffer::Create(GetApp().GetDevice(), textureDesc);

		m_luminosityRT = std::make_unique<SingleRenderTarget>(GetApp(), std::move(rtBuffer));
	}
}

//-------------------------------------------------------------------------

void ToneMappingComponent::SetExposure(const float exposure)
{
	m_exposure = exposure;
}

void ToneMappingComponent::SetGammaCorrection(const bool enabled)
{
	m_gammaCorrectionEnabled = enabled;
}

//-------------------------------------------------------------------------

void ToneMappingComponent::Draw(const Time& time)
{

}
} // namespace library
