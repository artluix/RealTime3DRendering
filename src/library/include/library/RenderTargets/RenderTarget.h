#pragma once
#include "library/RTTI.h"
#include "library/NonCopyable.h"
#include "library/Math/Color.h"

#include <stack>
#include <d3d11_1.h>

namespace library
{
class Application;

class RenderTarget : public NonCopyable<RenderTarget>
{
	RTTI_CLASS_BASE(RenderTarget)

public:
	struct ClearParams
	{
		math::Color rtvColor = colors::Black;

		UINT dsvFlags = D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL;
		float dsvDepth = 1.f;
		UINT8 dsvStencil = 0;

		ClearParams() = default;
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

	virtual void Begin() = 0;
	void Clear(const ClearParams& cp = ClearParams());
	void End();

protected:
	struct ViewData
	{
		ID3D11RenderTargetView**rtvs = nullptr;
		unsigned rtvsCount = 0;

		ID3D11DepthStencilView* dsv = nullptr;

		const D3D11_VIEWPORT* vps = nullptr;
		unsigned vpsCount = 0;
	};

	void Begin(const ViewData& viewData);

	virtual const Application& GetApp() const = 0;

private:
	void SetViewData(const ViewData& viewData);

	static std::stack<ViewData> s_viewsData;
};
} // namespace library
