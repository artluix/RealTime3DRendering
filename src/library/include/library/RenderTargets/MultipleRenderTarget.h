#pragma once
#include "library/RenderTargets/RenderTarget.h"
#include "library/Render/RenderBuffers.h"
#include "library/Render/Viewport.h"

namespace library
{
class MultipleRenderTarget : public RenderTarget
{
	RTTI_CLASS(MultipleRenderTarget, RenderTarget)

public:
	explicit MultipleRenderTarget(
		std::vector<RenderTargetBufferPtr> rtBuffers = {},
		DepthStencilBufferPtr dsBuffer = {},
		std::vector<ViewportPtr> vps = {}
	);

	unsigned GetRenderTargetBuffersCount() const { return unsigned(m_rtBuffers.size()); }
	const RenderTargetBuffer* GetRenderTargetBuffer(const unsigned idx) const;
	const DepthStencilBuffer* GetDepthStencilBuffer() const { return m_dsBuffer.get(); }

	void Begin() override;

protected:
	std::vector<RenderTargetBufferPtr> m_rtBuffers;
	DepthStencilBufferPtr m_dsBuffer;
	std::vector<ViewportPtr> m_vps;
};
} // namespace library
