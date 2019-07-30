#include "StdAfx.h"
#include "library/RenderTargets/FullScreenRenderTarget.h"

#include "library/Application.h"

namespace library
{
FullScreenRenderTarget::FullScreenRenderTarget(const Application& app) : m_app(app)
{
	auto device = app.GetDevice();

	// output texture & render target view
	{
		ComPtr<ID3D11Texture2D> fullScreenTexture;

		D3D11_TEXTURE2D_DESC fullScreenTextureDesc{};
		fullScreenTextureDesc.Width = app.GetScreenWidth();
		fullScreenTextureDesc.Height = app.GetScreenHeight();
		fullScreenTextureDesc.MipLevels = 1;
		fullScreenTextureDesc.ArraySize = 1;
		fullScreenTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		fullScreenTextureDesc.SampleDesc.Count = 1;
		fullScreenTextureDesc.SampleDesc.Quality = 0;
		fullScreenTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		fullScreenTextureDesc.Usage = D3D11_USAGE_DEFAULT;

		auto hr = device->CreateTexture2D(&fullScreenTextureDesc, nullptr, &fullScreenTexture);
		assert("ID3D11::CreateTexture2D() failed." && SUCCEEDED(hr));

		hr = device->CreateShaderResourceView(fullScreenTexture.Get(), nullptr, &m_outputTexture);
		assert("ID3D11::CreateShaderResourceView() failed." && SUCCEEDED(hr));

		hr = device->CreateRenderTargetView(fullScreenTexture.Get(), nullptr, &m_renderTargetView);
		assert("ID3D11::CreateRenderTargetView() failed." && SUCCEEDED(hr));
	}

	// depth stencil view
	{
		ComPtr<ID3D11Texture2D> depthStencilBuffer;

		D3D11_TEXTURE2D_DESC depthStencilDesc{};
		depthStencilDesc.Width = app.GetScreenWidth();
		depthStencilDesc.Height = app.GetScreenHeight();
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

		auto hr = device->CreateTexture2D(&depthStencilDesc, nullptr, &depthStencilBuffer);
		assert("ID3D11::CreateTexture2D() failed." && SUCCEEDED(hr));

		hr = device->CreateDepthStencilView(depthStencilBuffer.Get(), nullptr, &m_depthStencilView);
		assert("ID3D11::CreateDepthStencilView() failed." && SUCCEEDED(hr));
	}
}

FullScreenRenderTarget::~FullScreenRenderTarget() = default;

//-------------------------------------------------------------------------

void FullScreenRenderTarget::Begin()
{
	RenderTarget::Begin(
		m_app.GetDeviceContext(),
		Data(&m_renderTargetView, 1, m_depthStencilView.Get(), m_app.GetViewport())
	);
}

void FullScreenRenderTarget::End()
{
	RenderTarget::End(m_app.GetDeviceContext());
}
} // namespace library
