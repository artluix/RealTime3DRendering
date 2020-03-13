#pragma once
#include "library/RTTI.hpp"
#include "library/NonCopyable.hpp"
#include "library/Math/Color.h"

#include <stack>
#include <d3d11_1.h>

namespace library
{
class Application;
using RenderTargetViewArray = std::vector<ID3D11RenderTargetView*>;

class RenderTarget : public NonCopyable<RenderTarget>
{
	RTTI_CLASS_BASE(RenderTarget)

public:
	struct ClearParams
	{
		math::Color rtvColor;

		UINT dsvFlags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;
		float dsvDepth = 1.f;
		UINT8 dsvStencil = 0;

		ClearParams(const math::Color& rtvColor) : rtvColor(rtvColor) {} // no dsv
		ClearParams( // no rtvs
			const UINT dsvFlags,
			const float dsvDepth,
			const UINT dsvStencil
		)
			: dsvFlags(dsvFlags)
			, dsvDepth(dsvDepth)
			, dsvStencil(dsvStencil)
		{}
		ClearParams( // full
			const math::Color& rtvColor,
			const UINT dsvFlags,
			const float dsvDepth,
			const UINT dsvStencil
		)
			: rtvColor(rtvColor)
			, dsvFlags(dsvFlags)
			, dsvDepth(dsvDepth)
			, dsvStencil(dsvStencil)
		{}
	};

	//-------------------------------------------------------------------------

	RenderTarget() = default;
	~RenderTarget() = default;

	void Clear(const ClearParams& cp);
	void Begin();
	void End();

	virtual const Application& GetApp() const = 0;

	virtual RenderTargetViewArray GetRenderTargetViews() const { return {}; };
	virtual ID3D11DepthStencilView* GetDepthStencilView() const { return nullptr; }
	virtual const D3D11_VIEWPORT& GetViewport() const = 0;

protected:
	struct ViewData
	{
		RenderTargetViewArray rtvs;
		ID3D11DepthStencilView* dsv = nullptr;
		D3D11_VIEWPORT viewport;

		ViewData(const RenderTarget& rt)
			: rtvs(rt.GetRenderTargetViews())
			, dsv(rt.GetDepthStencilView())
			, viewport(rt.GetViewport())
		{}
	};

private:
	void SetViewData(const ViewData& viewData);

	static std::stack<ViewData> s_viewsData;
};
} // namespace library
