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
		auto& vp = m_vps.emplace_back(new Viewport());

		vp->TopLeftX = 0.f;
		vp->TopLeftY = 0.f;
		vp->Width = static_cast<float>(width);
		vp->Height = static_cast<float>(height);
		vp->MinDepth = 0.f;
		vp->MaxDepth = 1.f;
	}
}

DepthMapRenderTarget::~DepthMapRenderTarget() = default;
} // namespace library
