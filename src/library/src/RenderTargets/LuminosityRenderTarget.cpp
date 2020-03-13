#include "StdAfx.h"
#include "library/RenderTargets/LuminosityRenderTarget.h"

#include "library/Utils.h"
#include "library/Application.h"

namespace library
{
LuminosityRenderTarget::LuminosityRenderTarget(const Application& app)
	: SecondaryRenderTarget(app)
{
	m_mipLevelsCount = utils::GetMipLevelsCount(m_app.GetScreenWidth(), m_app.GetScreenHeight());

	auto device = app.GetDevice();

	// setup and create texture, SRV and RTV
	{
		ComPtr<ID3D11Texture2D> fullScreenTexture;

		D3D11_TEXTURE2D_DESC fullScreenTextureDesc{};
		fullScreenTextureDesc.Width = app.GetScreenWidth();
		fullScreenTextureDesc.Height = app.GetScreenHeight();
		fullScreenTextureDesc.MipLevels = m_mipLevelsCount;
		fullScreenTextureDesc.ArraySize = 1;
		fullScreenTextureDesc.Format = DXGI_FORMAT_R32_FLOAT; // need only 1 float value
		fullScreenTextureDesc.SampleDesc.Count = 1;
		fullScreenTextureDesc.SampleDesc.Quality = 0;
		fullScreenTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		fullScreenTextureDesc.Usage = D3D11_USAGE_DEFAULT;
		fullScreenTextureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		auto hr = device->CreateTexture2D(&fullScreenTextureDesc, nullptr, &fullScreenTexture);
		assert("ID3D11::CreateTexture2D() failed." && SUCCEEDED(hr));

		hr = device->CreateShaderResourceView(fullScreenTexture.Get(), nullptr, &m_outputTexture);
		assert("ID3D11::CreateShaderResourceView() failed." && SUCCEEDED(hr));

		hr = device->CreateRenderTargetView(fullScreenTexture.Get(), nullptr, &m_renderTargetView);
		assert("ID3D11::CreateRenderTargetView() failed." && SUCCEEDED(hr));
	}
}

LuminosityRenderTarget::~LuminosityRenderTarget() = default;

} // namespace library
