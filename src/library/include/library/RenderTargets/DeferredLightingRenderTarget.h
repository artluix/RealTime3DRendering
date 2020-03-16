#pragma once
#include "library/RenderTargets/RenderTarget.h"
#include "library/Render/RenderBuffers.h"
#include "library/Common.h"

namespace library
{
class DeferredLightingRenderTarget : public RenderTarget
{
	RTTI_CLASS(DeferredLightingRenderTarget, RenderTarget)

public:
	explicit DeferredLightingRenderTarget(const Application& app);
	~DeferredLightingRenderTarget();

	const RenderTargetBuffer* GetColorBuffer() const { return m_colorBuffer.get(); }
	const RenderTargetBuffer* GetNormalBuffer() const { return m_normalBuffer.get(); }
	const RenderTargetBuffer* GetWorldPositionBuffer() const { return m_worldPositionBuffer.get(); }

	void Begin() override final;

protected:
	const Application& GetApp() const { return m_app; }

private:
	const Application& m_app;

	RenderTargetBufferPtr m_colorBuffer;
	RenderTargetBufferPtr m_normalBuffer;
	RenderTargetBufferPtr m_worldPositionBuffer;
};

} // namespace library
