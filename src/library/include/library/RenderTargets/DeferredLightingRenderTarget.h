#pragma once
#include "library/RenderTargets/MultipleRenderTarget.h"

namespace library
{
class DeferredLightingRenderTarget : public MultipleRenderTarget
{
	RTTI_CLASS(DeferredLightingRenderTarget, RenderTarget)

public:
	explicit DeferredLightingRenderTarget(const Application& app);
	~DeferredLightingRenderTarget();

	const RenderTargetBuffer* GetColorBuffer() const { return GetRenderTargetBuffer(Color); }
	const RenderTargetBuffer* GetNormalBuffer() const { return GetRenderTargetBuffer(Normal); }
	const RenderTargetBuffer* GetWorldPositionBuffer() const { return GetRenderTargetBuffer(WorldPosition); }

private:
	enum BufferIdx : unsigned
	{
		Color = 0,
		Normal,
		WorldPosition
	};
};
} // namespace library
