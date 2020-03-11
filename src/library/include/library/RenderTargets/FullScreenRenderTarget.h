#pragma once
#include "library/RenderTargets/RenderTarget.h"
#include "library/Common.h"

namespace library
{
class Application;

// TODO: remove DSV from post-processing render targets

class FullScreenRenderTarget : public RenderTarget
{
	RTTI_CLASS(FullScreenRenderTarget, RenderTarget)

public:
	explicit FullScreenRenderTarget(const Application& app);
	~FullScreenRenderTarget();

	ID3D11RenderTargetView* GetRenderTargetView() const { return m_renderTargetView.Get(); }
	ID3D11ShaderResourceView* GetOutputTexture() const { return m_outputTexture.Get(); }

	void Clear(const ClearParams& cp) override;
	void Begin() override;
	void End() override;

private:
	const Application& m_app;

	ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	ComPtr<ID3D11ShaderResourceView> m_outputTexture;
};
} // namespace library
