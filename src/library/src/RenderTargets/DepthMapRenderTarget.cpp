#include "StdAfx.h"
#include "library/RenderTargets/DepthMapRenderTarget.h"

#include "library/Application.h"

namespace library
{
DepthMapRenderTarget::DepthMapRenderTarget(const Application& app, const unsigned width, const unsigned height)
	: SecondaryRenderTarget(app)
{
	auto device = app.GetDevice();

	ComPtr<ID3D11Texture2D> texture;

	// setup and create texture
	{
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		auto hr = device->CreateTexture2D(&textureDesc, nullptr, &texture);
		assert("ID3D11::CreateTexture2D() failed." && SUCCEEDED(hr));

		// setup and create SRV
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc{};
			resourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			resourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
			resourceViewDesc.Texture2D.MipLevels = 1;

			hr = device->CreateShaderResourceView(texture.Get(), &resourceViewDesc, &m_outputTexture);
			assert("ID3D11::CreateShaderResourceView() failed." && SUCCEEDED(hr));
		}
	}

	// setup and create DSV
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		auto hr = device->CreateDepthStencilView(texture.Get(), &depthStencilViewDesc, &m_depthStencilView);
		assert("ID3D11::CreateDepthStencilView() failed." && SUCCEEDED(hr));
	}

	// viewport
	{
		m_viewport.TopLeftX = 0.f;
		m_viewport.TopLeftY = 0.f;
		m_viewport.Width = static_cast<float>(width);
		m_viewport.Height = static_cast<float>(height);
		m_viewport.MinDepth = 0.f;
		m_viewport.MaxDepth = 1.f;
	}
}

DepthMapRenderTarget::~DepthMapRenderTarget() = default;

} // namespace library
