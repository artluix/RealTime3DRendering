#pragma once
#include "library/RenderTargets/RenderTarget.h"

namespace library
{
class CombinedRenderTarget : public RenderTarget
{
	RTTI_CLASS(CombinedRenderTarget, RenderTarget)

public:
	template<typename First, typename Second, typename ... Others>
	explicit CombinedRenderTarget(const First& firstRT, const Second& secondRT, Others... othersRT)
		: m_firstRT(firstRT)
	{
		Init(firstRT, secondRT, std::forward<Others>(othersRT)...);
	}

	RenderTargetViewArray GetRenderTargetViews() const override final { return m_renderTargetViews; }
	ID3D11DepthStencilView* GetDepthStencilView() const override final { return m_depthStencilView; }
	const Application& GetApp() const override final { return m_firstRT.GetApp(); }
	const D3D11_VIEWPORT& GetViewport() const override final { return m_firstRT.GetViewport(); }

private:
	template<typename First, typename Second, typename ... Others>
	void Init(const First& firstRT, const Second& secondRT, Others... othersRT)
	{
		Init(firstRT);
		Init(secondRT, std::forward<Others>(othersRT)...);
	}

	template<typename RenderTargetType>
	void Init(const RenderTargetType& rt)
	{
		static_assert(std::is_base_of_v(RenderTarget, RenderTargetType),
			"RenderTargetType must be derived from RenderTarget");

		auto rtvs = rt.GetRenderTargetViews();
		m_renderTargetViews.insert(m_renderTargetViews.end(), rtvs.begin(), rtvs.end());

		// should be only 1 DSV
		assert(!m_depthStencilView || !rt.GetDepthStencilView());

		if (!m_depthStencilView && !!rt.GetDepthStencilView())
		{
			m_depthStencilView = rt.GetDepthStencilView();
		}
	}

	const RenderTarget& m_firstRT;

	RenderTargetViewArray m_renderTargetViews;
	ID3D11DepthStencilView* m_depthStencilView = nullptr;
};
} // namespace library
