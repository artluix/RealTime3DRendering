#pragma once
#include "library/RenderTargets/RenderTarget.h"
#include "library/Common.h"

namespace library
{
class Application;

class HdrRenderTarget : public RenderTarget
{
	RTTI_CLASS(FullScreenRenderTarget, RenderTarget)

public:
	explicit HdrRenderTarget(const Application& app);
	~HdrRenderTarget();

	ID3D11RenderTargetView* GetRenderTargetView() const { return m_renderTargetView.Get(); }
	ID3D11DepthStencilView* GetDepthStencilView() const { return m_depthStencilView.Get(); }
	ID3D11ShaderResourceView* GetOutputTexture() const { return m_outputTexture.Get(); }

	void Begin() override;
	void End() override;

private:
	const Application& m_app;

	ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11ShaderResourceView> m_outputTexture;
};
} // namespace library
