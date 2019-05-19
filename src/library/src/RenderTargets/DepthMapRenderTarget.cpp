#include "StdAfx.h"
#include "library/RenderTargets/DepthMapRenderTarget.h"

#include "library/Application.h"
#include "library/Exception.h"

namespace library
{
DepthMapRenderTarget::DepthMapRenderTarget(const Application& app, const unsigned width, const unsigned height)
	: m_app(app)
{
	auto device = app.GetDevice();

	ComPtr<ID3D11Texture2D> texture;

	// output texture
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
		if (FAILED(hr))
		{
			throw Exception("ID3D11::CreateTexture2D() failed.", hr);
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc{};
		resourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		resourceViewDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		resourceViewDesc.Texture2D.MipLevels = 1;

		hr = device->CreateShaderResourceView(texture.Get(), &resourceViewDesc, &m_outputTexture);
		if (FAILED(hr))
		{
			throw Exception("ID3D11::CreateShaderResourceView() failed.", hr);
		}
	}

	// depth stencil view
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{};
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		auto hr = device->CreateDepthStencilView(texture.Get(), &depthStencilViewDesc, &m_depthStencilView);
		if (FAILED(hr))
		{
			throw Exception("ID3D11::CreateDepthStencilView() failed.", hr);
		}
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

//-------------------------------------------------------------------------

void DepthMapRenderTarget::Begin()
{
	static ID3D11RenderTargetView* nullRenderTargetView = nullptr;

	RenderTarget::
		Begin(m_app.GetDeviceContext(), Data(nullRenderTargetView, m_depthStencilView.Get(), m_viewport));
}

void DepthMapRenderTarget::End()
{
	RenderTarget::End(m_app.GetDeviceContext());
}
} // namespace library
