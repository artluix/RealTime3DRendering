#include "StdAfx.h"
#include "library/RenderTargets/MultipleRenderTarget.h"

namespace library
{
MultipleRenderTarget::MultipleRenderTarget(
	std::vector<RenderTargetBufferPtr> rtBuffers /*= {}*/,
	DepthStencilBufferPtr dsBuffer /*= {}*/,
	std::vector<ViewportPtr> vps /*= {} */)
	: m_rtBuffers(std::move(rtBuffers))
	, m_dsBuffer(std::move(dsBuffer))
	, m_vps(std::move(vps))
{
}

//-------------------------------------------------------------------------

const RenderTargetBuffer* MultipleRenderTarget::GetRenderTargetBuffer(const unsigned idx) const
{
	assert(idx < m_rtBuffers.size());
	return m_rtBuffers[idx].get();
}

void MultipleRenderTarget::Begin()
{
	ViewData viewData;

	if (!m_rtBuffers.empty())
	{
		viewData.rtvsCount = m_rtBuffers.size();

		std::vector<ID3D11RenderTargetView*> rtvs;
		rtvs.reserve(viewData.rtvsCount);

		for (auto& rtBuffer : m_rtBuffers)
		{
			rtvs.emplace_back(rtBuffer->GetView());
		}

		viewData.rtvs = rtvs.data();
	}

	if (!!m_dsBuffer)
		viewData.dsv = m_dsBuffer->GetView();

	if (!m_vps.empty())
	{
		viewData.vpsCount = m_vps.size();

		std::vector<Viewport> vps;
		vps.reserve(viewData.vpsCount);

		for (auto& vp : m_vps)
		{
			vps.emplace_back(*vp);
		}

		viewData.vps = vps.data();
	}

	RenderTarget::Begin(viewData);
}

} // namespace library
