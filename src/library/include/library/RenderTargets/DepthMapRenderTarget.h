#pragma once
#include "library/RenderTargets/SecondaryRenderTarget.h"
#include "library/Common.h"

namespace library
{
class DepthMapRenderTarget : public SecondaryRenderTarget
{
	RTTI_CLASS(DepthMapRenderTarget, SecondaryRenderTarget)

public:
	DepthMapRenderTarget(const Application& app, const unsigned width, const unsigned height);
	~DepthMapRenderTarget();

	ID3D11ShaderResourceView* GetOutputTexture() const { return m_outputTexture.Get(); }

	ID3D11DepthStencilView* GetDepthStencilView() const override final { return m_depthStencilView.Get(); }
	const D3D11_VIEWPORT& GetViewport() const override final { return m_viewport; }

private:
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11ShaderResourceView> m_outputTexture;
	D3D11_VIEWPORT m_viewport;
};
} // namespace library
