#include "StdAfx.h"
#include "library/RenderTargets/SingleRenderTarget.h"

namespace library
{
SingleRenderTarget::SingleRenderTarget(
	RenderTargetBufferPtr rtBuffer /*= {}*/,
	DepthStencilBufferPtr dsBuffer /*= {}*/,
	ViewportPtr vp /*= {}*/)
	: MultipleRenderTarget({ std::move(rtBuffer) }, std::move(dsBuffer), { std::move(vp) })
{
}

} // namespace library
