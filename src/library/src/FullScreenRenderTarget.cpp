#include "StdAfx.h"
#include "library/FullScreenRenderTarget.h"

#include "library/Application.h"
#include "library/Exception.h"

namespace library
{
	FullScreenRenderTarget::FullScreenRenderTarget(const Application& app)
		: m_app(app)
	{
		auto device = m_app.GetDevice();

		// output texture & render target view
		{
			ComPtr<ID3D11Texture2D> fullScreenTexture;

			D3D11_TEXTURE2D_DESC fullScreenTextureDesc{};
			fullScreenTextureDesc.Width = m_app.GetScreenWidth();
			fullScreenTextureDesc.Height = m_app.GetScreenHeight();
			fullScreenTextureDesc.MipLevels = 1;
			fullScreenTextureDesc.ArraySize = 1;
			fullScreenTextureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			fullScreenTextureDesc.SampleDesc.Count = 1;
			fullScreenTextureDesc.SampleDesc.Quality = 0;
			fullScreenTextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			fullScreenTextureDesc.Usage = D3D11_USAGE_DEFAULT;

			auto hr = device->CreateTexture2D(
				&fullScreenTextureDesc,
				nullptr,
				fullScreenTexture.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("ID3D11::CreateTexture2D() failed.", hr);
			}

			hr = device->CreateShaderResourceView(
				fullScreenTexture.Get(),
				nullptr,
				m_outputTexture.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("ID3D11::CreateShaderResourceView() failed.", hr);
			}

			hr = device->CreateRenderTargetView(
				fullScreenTexture.Get(),
				nullptr,
				m_renderTargetView.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("ID3D11::CreateRenderTargetView() failed.", hr);
			}
		}

		// depth stencil view
		{
			ComPtr<ID3D11Texture2D> depthStencilBuffer;

			D3D11_TEXTURE2D_DESC depthStencilDesc{};
			depthStencilDesc.Width = m_app.GetScreenWidth();
			depthStencilDesc.Height = m_app.GetScreenHeight();
			depthStencilDesc.MipLevels = 1;
			depthStencilDesc.ArraySize = 1;
			depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilDesc.SampleDesc.Count = 1;
			depthStencilDesc.SampleDesc.Quality = 0;
			depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

			auto hr = device->CreateTexture2D(
				&depthStencilDesc,
				nullptr,
				depthStencilBuffer.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("ID3D11::CreateTexture2D() failed.", hr);
			}

			hr = device->CreateDepthStencilView(
				depthStencilBuffer.Get(),
				nullptr,
				m_depthStencilView.GetAddressOf()
			);
			if (FAILED(hr))
			{
				throw Exception("ID3D11::CreateDepthStencilView() failed.", hr);
			}
		}
	}

	FullScreenRenderTarget::~FullScreenRenderTarget() = default;

	void FullScreenRenderTarget::Begin()
	{
		m_app.GetDeviceContext()->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());
	}

	void FullScreenRenderTarget::End()
	{
		m_app.ResetRenderTargets();
	}
} // namespace library
