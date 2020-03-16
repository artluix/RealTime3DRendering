#pragma once
#include "library/RenderTargets/RenderTarget.h"
#include "library/Render/RenderBuffers.h"
#include "library/Common.h"

namespace library
{
class MultipleRenderTarget : public RenderTarget
{
	RTTI_CLASS(MultipleRenderTarget, RenderTarget)

public:
	explicit MultipleRenderTarget(const Application& app, const unsigned size);
	~MultipleRenderTarget();

	const RenderTargetBuffer* GetRenderTargetBuffer(const unsigned idx) const { return m_rtBuffers[idx].get(); }

	void Begin() override final;

protected:
	const Application& GetApp() const { return m_app; }

private:
	const Application& m_app;

	std::vector<RenderTargetBufferPtr> m_rtBuffers;
};

} // namespace library
