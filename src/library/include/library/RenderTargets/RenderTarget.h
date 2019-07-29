#pragma once
#include "library/RTTI.hpp"
#include "library/NonCopyable.hpp"

#include <stack>
#include <d3d11_1.h>

namespace library
{
class RenderTarget : public NonCopyable<RenderTarget>
{
	RTTI_CLASS_BASE(RenderTarget)

public:
	RenderTarget() = default;
	~RenderTarget() = default;

	virtual void Begin() = 0;
	virtual void End() = 0;

protected:
	struct Data
	{
		Data(ID3D11RenderTargetView** rtvs, const unsigned rtvsCount, ID3D11DepthStencilView* dsv, const D3D11_VIEWPORT& vp);
		Data(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv, const D3D11_VIEWPORT& vp);

		ID3D11RenderTargetView** renderTargetViews = nullptr;
		unsigned renderTargetViewsCount;
		ID3D11DepthStencilView* depthStencilView = nullptr;
		D3D11_VIEWPORT viewport;
	};

	void Begin(ID3D11DeviceContext* const deviceContext, const Data& data);
	void End(ID3D11DeviceContext* const deviceContext);

private:
	void SetRenderTargetData(ID3D11DeviceContext* const deviceContext, const Data& data);

	static std::stack<Data> s_renderTargetsData;
};
} // namespace library
