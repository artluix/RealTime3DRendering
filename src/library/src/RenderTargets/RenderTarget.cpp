#include "StdAfx.h"
#include "library/RenderTargets/RenderTarget.h"

namespace library
{
std::stack<library::RenderTarget::ViewData> RenderTarget::s_viewsData;

//-------------------------------------------------------------------------

void RenderTarget::Clear(ID3D11DeviceContext* const deviceContext, const ClearParams& cp)
{
	// we will take views from data, because it must be already pushed
	if (!s_viewsData.empty())
	{
		const auto& viewData = s_viewsData.top();

		for (unsigned i = 0; i < viewData.rtvsCount; i++)
		{
			deviceContext->ClearRenderTargetView(viewData.rtvs[i],  static_cast<const float*>(cp.rtvColor));
		}

		if (!!viewData.dsv)
		{
			deviceContext->ClearDepthStencilView(viewData.dsv, cp.dsvFlags, cp.dsvDepth, cp.dsvStencil);
		}
	}
}

//-------------------------------------------------------------------------

void RenderTarget::Begin(ID3D11DeviceContext* const deviceContext, const ViewData& data)
{
	s_viewsData.push(data);
	SetViewData(deviceContext, data);
}

void RenderTarget::End(ID3D11DeviceContext* const deviceContext)
{
	s_viewsData.pop();

	if (!s_viewsData.empty())
	{
		SetViewData(deviceContext, s_viewsData.top());
	}
}

//-------------------------------------------------------------------------

void RenderTarget::SetViewData(ID3D11DeviceContext* const deviceContext, const ViewData& viewData)
{
	deviceContext->OMSetRenderTargets(
		viewData.rtvsCount,
		viewData.rtvs,
		viewData.dsv
	);
	deviceContext->RSSetViewports(1, &viewData.viewport);
}
} // namespace library
