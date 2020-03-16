#pragma once
#include "library/RenderTargets/SingleRenderTarget.h"

namespace library
{
class DepthMapRenderTarget : public SingleRenderTarget
{
	RTTI_CLASS(DepthMapRenderTarget, SingleRenderTarget)

public:
	DepthMapRenderTarget(const Application& app, const unsigned width, const unsigned height);
	~DepthMapRenderTarget();

	void Begin() override final;

private:
	D3D11_VIEWPORT m_viewport;
};
} // namespace library
