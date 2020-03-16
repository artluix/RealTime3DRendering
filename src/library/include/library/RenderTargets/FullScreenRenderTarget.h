#pragma once
#include "library/RenderTargets/SingleRenderTarget.h"
#include "library/Common.h"

namespace library
{
class FullScreenRenderTarget : public SingleRenderTarget
{
	RTTI_CLASS(FullScreenRenderTarget, SingleRenderTarget)

public:
	explicit FullScreenRenderTarget(const Application& app);
	~FullScreenRenderTarget();
};
} // namespace library
