#include "StdAfx.h"
#include "library/RenderTargets/MultipleRenderTarget.h"

#include "library/Application.h"

namespace library
{
MultipleRenderTarget::MultipleRenderTarget(const Application& app, const unsigned size)
	: m_app(app)
{
	auto device = app.GetDevice();

	// output texture & render target view
	{
		for (unsigned i = 0; i < size; i++)
		{
			ComPtr<ID3D11Texture2D> texture;

			D3D11_TEXTURE2D_DESC textureDesc{};
			textureDesc.Width = app.GetScreenWidth();
			textureDesc.Height = app.GetScreenHeight();
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			textureDesc.Usage = D3D11_USAGE_DEFAULT;

			auto hr = device->CreateTexture2D(&textureDesc, nullptr, &texture);
			assert("ID3D11::CreateTexture2D() failed." && SUCCEEDED(hr));

			auto& outputTexture = m_outputTextures.emplace_back();
			hr = device->CreateShaderResourceView(texture.Get(), nullptr, &outputTexture);
			assert("ID3D11::CreateShaderResourceView() failed." && SUCCEEDED(hr));

			auto& renderTargetView = m_renderTargetViews.emplace_back();
			hr = device->CreateRenderTargetView(texture.Get(), nullptr, &renderTargetView);
			assert("ID3D11::CreateRenderTargetView() failed." && SUCCEEDED(hr));
		}
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

MultipleRenderTarget::~MultipleRenderTarget() = default;

//-------------------------------------------------------------------------

void MultipleRenderTarget::Begin()
{
	// avoid reinterpret_cast from std::vector<ComPtr<ID3D11RenderTargetView>> to ID3D11RenderTargetView**
	std::vector<ID3D11RenderTargetView*> rtvs;
	rtvs.reserve(m_renderTargetViews.size());

	for (const auto& rtv : m_renderTargetViews)
	{
		rtvs.push_back(rtv.Get());
	}

	RenderTarget::Begin(
		m_app.GetDeviceContext(),
		Data(
			rtvs.data(), m_renderTargetViews.size(),
			m_depthStencilView.Get(),
			m_app.GetViewport()
		)
	);
}

void MultipleRenderTarget::End()
{
	RenderTarget::End(m_app.GetDeviceContext());
}
} // namespace library
