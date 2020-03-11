#pragma once
#include "library/RenderTargets/RenderTarget.h"
#include "library/Common.h"

namespace library
{
class Application;

class DepthMapRenderTarget : public RenderTarget
{
	RTTI_CLASS(DepthMapRenderTarget, RenderTarget)

public:
	DepthMapRenderTarget(const Application& app, const unsigned width, const unsigned height);
	~DepthMapRenderTarget();

	ID3D11DepthStencilView* GetDepthStencilView() const { return m_depthStencilView.Get(); }
	ID3D11ShaderResourceView* GetOutputTexture() const { return m_outputTexture.Get(); }
	const D3D11_VIEWPORT& GetViewport() const { return m_viewport; }

	void Clear(const ClearParams& cp);
	void Begin() override;
	void End() override;

private:
	const Application& m_app;

	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11ShaderResourceView> m_outputTexture;
	D3D11_VIEWPORT m_viewport;
};
} // namespace library
