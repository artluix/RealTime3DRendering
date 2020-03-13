#pragma once
#include "library/RenderTargets/RenderTarget.h"

namespace library
{
class SecondaryRenderTarget : public RenderTarget
{
	RTTI_CLASS(SecondaryRenderTarget, RenderTarget)

public:
	explicit SecondaryRenderTarget(const Application& app) : m_app(app) {}

	const Application& GetApp() const override final { return m_app; }
	const D3D11_VIEWPORT& GetViewport() const override;

protected:
	const Application& m_app;
};
} // namespace library
