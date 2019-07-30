#include "StdAfx.h"
#include "library/RenderTargets/RenderTarget.h"

namespace library
{
std::stack<library::RenderTarget::Data> RenderTarget::s_renderTargetsData;

//-------------------------------------------------------------------------

RenderTarget::Data::Data(ID3D11RenderTargetView* const *rtvs, const unsigned rtvsCount, ID3D11DepthStencilView* dsv, const D3D11_VIEWPORT& vp)
	: renderTargetViews(rtvs)
	, renderTargetViewsCount(rtvsCount)
	, depthStencilView(dsv)
	, viewport(vp)
{
}

//-------------------------------------------------------------------------

void RenderTarget::Begin(ID3D11DeviceContext* const deviceContext, const Data& data)
{
	s_renderTargetsData.push(data);
	SetRenderTargetData(deviceContext, data);
}

void RenderTarget::End(ID3D11DeviceContext* const deviceContext)
{
	s_renderTargetsData.pop();

	if (!s_renderTargetsData.empty())
	{
		SetRenderTargetData(deviceContext, s_renderTargetsData.top());
	}
}

//-------------------------------------------------------------------------

void RenderTarget::SetRenderTargetData(ID3D11DeviceContext* const deviceContext, const Data& data)
{
	deviceContext->OMSetRenderTargets(
		data.renderTargetViewsCount,
		data.renderTargetViews,
		data.depthStencilView
	);
	deviceContext->RSSetViewports(1, &data.viewport);
}
} // namespace library
