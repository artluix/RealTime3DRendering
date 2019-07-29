#pragma once
#include "library/RenderTargets/RenderTarget.h"
#include "library/Common.h"

#include <vector>

namespace library
{
class Application;

class MultipleRenderTarget : public RenderTarget
{
	RTTI_CLASS(MultipleRenderTarget, RenderTarget)

public:
	explicit MultipleRenderTarget(const Application& app, const unsigned size);
	~MultipleRenderTarget();

	ID3D11RenderTargetView* GetRenderTargetView(const unsigned idx) const;
	ID3D11ShaderResourceView* GetOutputTexture(const unsigned idx) const;
	ID3D11DepthStencilView* GetDepthStencilView() const { return m_depthStencilView.Get(); }

	void Begin() override;
	void End() override;

private:
	std::vector<ComPtr<ID3D11RenderTargetView>> m_renderTargetViews;
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_outputTextures;
	ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	const Application& m_app;
};

//-------------------------------------------------------------------------

inline ID3D11RenderTargetView* MultipleRenderTarget::GetRenderTargetView(const unsigned idx) const
{
	return m_renderTargetViews[idx].Get();
}

inline ID3D11ShaderResourceView* MultipleRenderTarget::GetOutputTexture(const unsigned idx) const
{
	return m_outputTextures[idx].Get();
}

} // namespace library
