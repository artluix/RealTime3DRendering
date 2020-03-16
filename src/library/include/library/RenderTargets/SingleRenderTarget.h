#pragma once
#include "library/RenderTargets/RenderTarget.h"
#include "library/Render/RenderBuffers.h"

namespace library
{
class SingleRenderTarget : public RenderTarget
{
	RTTI_CLASS(SingleRenderTarget, RenderTarget)

public:
	explicit SingleRenderTarget(
		const Application& app,
		RenderTargetBufferPtr rtBuffer = {},
		DepthStencilBufferPtr dsBuffer = {});
	SingleRenderTarget(const Application& app, DepthStencilBufferPtr dsBuffer);

	const RenderTargetBuffer* GetRenderTargetBuffer() const { return m_rtBuffer.get(); }
	const DepthStencilBuffer* GetDepthStencilBuffer() const { return m_dsBuffer.get(); }

	void Begin() override;

protected:
	const Application& GetApp() const override final { return m_app; }

	ViewData CreateViewDataFromBuffers() const;

	const Application& m_app;
	RenderTargetBufferPtr m_rtBuffer;
	DepthStencilBufferPtr m_dsBuffer;
};
} // namespace library
