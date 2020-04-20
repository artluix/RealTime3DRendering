#pragma once
#include "library/RenderTargets/MultipleRenderTarget.h"

namespace library
{
class SingleRenderTarget : public MultipleRenderTarget
{
	RTTI_CLASS(SingleRenderTarget, MultipleRenderTarget)

public:
	explicit SingleRenderTarget(
		RenderTargetBufferPtr rtBuffer = {},
		DepthStencilBufferPtr dsBuffer = {},
		ViewportPtr vp = {}
	);

	const RenderTargetBuffer* GetRenderTargetBuffer() const { return m_rtBuffers[0].get(); }
	const DepthStencilBuffer* GetDepthStencilBuffer() const { return m_dsBuffer.get(); }
	const Viewport* GetViewport() const { return m_vps[0].get(); }
};
} // namespace library
