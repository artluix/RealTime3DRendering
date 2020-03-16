#include "StdAfx.h"
#include "library/RenderTargets/DepthMapRenderTarget.h"

#include "library/Application.h"

namespace library
{
DepthMapRenderTarget::DepthMapRenderTarget(const Application& app, const unsigned width, const unsigned height)
	: SingleRenderTarget(app)
{
	// setup depth buffer
	{
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;

		auto device = app.GetDevice();
		m_dsBuffer = DepthStencilBuffer::Create(device, textureDesc, &dsvDesc, &srvDesc);
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
	ViewData viewData = CreateViewDataFromBuffers();

	viewData.vpsCount = 1;
	viewData.vps = &m_viewport;

	RenderTarget::Begin(viewData);
}

} // namespace library
