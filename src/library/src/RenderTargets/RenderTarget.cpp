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

		for (auto& rtv : viewData.rtvs)
		{
			deviceContext->ClearRenderTargetView(rtv, static_cast<const float*>(cp.rtvColor));
		}

		if (!!viewData.dsv)
		{
			deviceContext->ClearDepthStencilView(viewData.dsv, cp.dsvFlags, cp.dsvDepth, cp.dsvStencil);
		}
	}
}

//-------------------------------------------------------------------------

void RenderTarget::Begin()
{
	ViewData data(*this);
	s_viewsData.push(data);
	SetViewData(data);
}

void RenderTarget::End()
{
	s_viewsData.pop();

	if (!s_viewsData.empty())
	{
		SetViewData(s_viewsData.top());
	}
}

//-------------------------------------------------------------------------

void RenderTarget::SetViewData(const ViewData& viewData)
{
	auto deviceContext = GetApp().GetDeviceContext();

	deviceContext->OMSetRenderTargets(
		unsigned(viewData.rtvs.size()),
		viewData.rtvs.data(),
		viewData.dsv
	);
	deviceContext->RSSetViewports(1, &viewData.viewport);
}
} // namespace library
