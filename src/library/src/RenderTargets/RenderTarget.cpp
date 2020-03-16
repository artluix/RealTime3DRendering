#include "StdAfx.h"
#include "library/RenderTargets/RenderTarget.h"

#include "library/Application.h"

namespace library
{
std::stack<library::RenderTarget::ViewData> RenderTarget::s_viewsData;

//-------------------------------------------------------------------------

void RenderTarget::Clear(const ClearParams& cp)
{
	// we will take views from data, because it must be already pushed
	if (!s_viewsData.empty())
	{
		auto deviceContext = GetApp().GetDeviceContext();

		const auto& viewData = s_viewsData.top();

		for (unsigned i = 0; i < viewData.rtvsCount; i++)
		{
			deviceContext->ClearRenderTargetView(viewData.rtvs[i], static_cast<const float*>(cp.rtvColor));
		}

		if (!!viewData.dsv)
		{
			deviceContext->ClearDepthStencilView(viewData.dsv, cp.dsvFlags, cp.dsvDepth, cp.dsvStencil);
		}
	}
}

//-------------------------------------------------------------------------

void RenderTarget::End()
{
	s_viewsData.pop();

	if (!s_viewsData.empty())
	{
		SetViewData(s_viewsData.top());
	}
}

void RenderTarget::Begin(const ViewData& viewData)
{
	SetViewData(viewData);

	auto vd = viewData;

	auto deviceContext = GetApp().GetDeviceContext();
	deviceContext->OMGetRenderTargets(vd.rtvsCount, vd.rtvs, &vd.dsv);
	s_viewsData.push(vd);
}

//-------------------------------------------------------------------------

void RenderTarget::SetViewData(const ViewData& viewData)
{
	auto deviceContext = GetApp().GetDeviceContext();

	deviceContext->OMSetRenderTargets(
		viewData.rtvsCount,
		viewData.rtvs,
		viewData.dsv
	);
	deviceContext->RSSetViewports(viewData.vpsCount, viewData.vps);
}
} // namespace library
