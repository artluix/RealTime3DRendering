#include "StdAfx.h"
#include "library/RenderTargets/RenderTarget.h"

namespace library
{
std::stack<library::RenderTarget::Data> RenderTarget::s_renderTargetsData;

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
	deviceContext->OMSetRenderTargets(1, &data.renderTargetView, data.depthStencilView);
	deviceContext->RSSetViewports(1, &data.viewport);
}
} // namespace library
