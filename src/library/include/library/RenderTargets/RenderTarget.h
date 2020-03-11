#pragma once
#include "library/RTTI.hpp"
#include "library/NonCopyable.hpp"
#include "library/Math/Color.h"

#include <stack>
#include <d3d11_1.h>

namespace library
{
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

	virtual void Clear(const ClearParams& cp) = 0;
	virtual void Begin() = 0;
	virtual void End() = 0;

protected:
	struct ViewData
	{
		ID3D11RenderTargetView* const *rtvs = nullptr;
		unsigned rtvsCount;
		ID3D11DepthStencilView* dsv = nullptr;
		D3D11_VIEWPORT viewport;

		ViewData(ID3D11RenderTargetView* const *rtvs, const unsigned rtvsCount, ID3D11DepthStencilView* dsv, const D3D11_VIEWPORT& vp)
			: rtvs(rtvs)
			, rtvsCount(rtvsCount)
			, dsv(dsv)
			, viewport(vp)
		{}
	};

	static void Clear(ID3D11DeviceContext* const deviceContext, const ClearParams& cp);
	static void Begin(ID3D11DeviceContext* const deviceContext, const ViewData& viewData);
	static void End(ID3D11DeviceContext* const deviceContext);

private:
	static void SetViewData(ID3D11DeviceContext* const deviceContext, const ViewData& viewData);

	static std::stack<ViewData> s_viewsData;
};
} // namespace library
