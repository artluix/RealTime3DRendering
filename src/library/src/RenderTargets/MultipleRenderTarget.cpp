#include "StdAfx.h"
#include "library/RenderTargets/MultipleRenderTarget.h"

#include "library/Application.h"

namespace library
{
MultipleRenderTarget::MultipleRenderTarget(const Application& app, const unsigned size)
	: m_app(app)
{
}

MultipleRenderTarget::~MultipleRenderTarget() = default;

//-------------------------------------------------------------------------

void MultipleRenderTarget::Begin()
{
	std::vector<ID3D11RenderTargetView*> rtvs;
	rtvs.reserve(m_renderTargetViews.size());

	for (const auto& rtv : m_renderTargetViews)
	{
		rtvs.push_back(rtv.Get());
	}

	RenderTarget::Begin(
		m_app.GetDeviceContext(),
		Data(
			rtvs.data(), m_renderTargetViews.size(),
			m_depthStencilView.Get(),
			m_app.GetViewport()
		)
	);
}

void MultipleRenderTarget::End()
{
	RenderTarget::End(m_app.GetDeviceContext());
}
} // namespace library
