#include "StdAfx.h"
#include "library/RenderTargets/FullScreenRenderTarget.h"

#include "library/Application.h"

namespace library
{
FullScreenRenderTarget::FullScreenRenderTarget(const Application& app)
	: SecondaryRenderTarget(app)
{
	auto device = app.GetDevice();

	// descs for RTV an DSV must have same params (Width, Height, Sampling)

	// setup texture for RTV
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = app.GetScreenWidth();
	textureDesc.Height = app.GetScreenHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	// create texture, SRV and RTV
	{
		ComPtr<ID3D11Texture2D> fullScreenTexture;

		auto hr = device->CreateTexture2D(&textureDesc, nullptr, &fullScreenTexture);
		assert("ID3D11::CreateTexture2D() failed." && SUCCEEDED(hr));

		hr = device->CreateShaderResourceView(fullScreenTexture.Get(), nullptr, &m_outputTexture);
		assert("ID3D11::CreateShaderResourceView() failed." && SUCCEEDED(hr));

		hr = device->CreateRenderTargetView(fullScreenTexture.Get(), nullptr, &m_renderTargetView);
		assert("ID3D11::CreateRenderTargetView() failed." && SUCCEEDED(hr));
	}

	// we don't need multisampling, so we need to create another DSV

	// modify texture desc for DSV
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	//textureDesc.Usage = D3D11_USAGE_DEFAULT;

	// create texture and DSV
	{
		ComPtr<ID3D11Texture2D> depthStencilBuffer;

		auto hr = device->CreateTexture2D(&textureDesc, nullptr, &depthStencilBuffer);
		assert("ID3D11::CreateTexture2D() failed." && SUCCEEDED(hr));

		hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, &m_depthStencilView);
		assert("ID3D11::CreateDepthStencilView() failed." && SUCCEEDED(hr));
	}
}

FullScreenRenderTarget::~FullScreenRenderTarget() = default;

} // namespace library
