#include "StdAfx.h"
#include "library/RenderTargets/SingleRenderTarget.h"

#include "library/Application.h"

namespace library
{
SingleRenderTarget::SingleRenderTarget(
	const Application& app,
	RenderTargetBufferPtr rtBuffer /*= {}*/,
	DepthStencilBufferPtr dsBuffer /*= {}*/)
	: m_app(app)
	, m_rtBuffer(std::move(rtBuffer))
	, m_dsBuffer(std::move(dsBuffer))
{
}

SingleRenderTarget::SingleRenderTarget(const Application& app, DepthStencilBufferPtr dsBuffer)
	: SingleRenderTarget(app, {}, std::move(dsBuffer))
{
}

//-------------------------------------------------------------------------

RenderTarget::ViewData SingleRenderTarget::CreateViewDataFromBuffers() const
{
	ViewData viewData;

	if (!!m_rtBuffer)
	{
		viewData.rtvsCount = 1;
		viewData.rtvs = &m_rtBuffer->GetView();
	}

	if (!!m_dsBuffer)
		viewData.dsv = m_dsBuffer->GetView();

	return viewData;
}

//-------------------------------------------------------------------------

void SingleRenderTarget::Begin()
{
	ViewData viewData = CreateViewDataFromBuffers();

	// set default viewport of app
	viewData.vpsCount = 1;
	viewData.vps = &GetApp().GetViewport();

	RenderTarget::Begin(viewData);
}

} // namespace library
