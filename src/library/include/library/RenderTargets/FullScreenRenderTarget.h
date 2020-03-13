#pragma once
#include "library/RenderTargets/SecondaryRenderTarget.h"
#include "library/Common.h"

namespace library
{
class FullScreenRenderTarget : public SecondaryRenderTarget
{
	RTTI_CLASS(FullScreenRenderTarget, SecondaryRenderTarget)

public:
	explicit FullScreenRenderTarget(const Application& app);
	~FullScreenRenderTarget();

	ID3D11ShaderResourceView* GetOutputTexture() const { return m_outputTexture.Get(); }

	RenderTargetViewArray GetRenderTargetViews() const { return { m_renderTargetView.Get() }; }
	ID3D11DepthStencilView* GetDepthStencilView() const override final { return m_depthStencilView.Get(); }

private:
	ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11ShaderResourceView> m_outputTexture;
};
} // namespace library
