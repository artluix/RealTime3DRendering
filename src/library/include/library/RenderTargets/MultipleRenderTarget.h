#pragma once
#include "library/RenderTargets/SecondaryRenderTarget.h"
#include "library/Common.h"

namespace library
{
class MultipleRenderTarget : public SecondaryRenderTarget
{
	RTTI_CLASS(MultipleRenderTarget, SecondaryRenderTarget)

public:
	explicit MultipleRenderTarget(const Application& app, const unsigned size);
	~MultipleRenderTarget();

	RenderTargetViewArray GetRenderTargetViews() const override final;
	ID3D11DepthStencilView* GetDepthStencilView() const override final { return m_depthStencilView.Get(); }

	ID3D11RenderTargetView* GetRenderTargetView(const unsigned idx) const;
	ID3D11ShaderResourceView* GetOutputTexture(const unsigned idx) const;

	ID3D11ShaderResourceView* GetDepthOutputTexture() const { return m_depthOutputTexture.Get(); }

private:
	std::vector<ComPtr<ID3D11RenderTargetView>> m_renderTargetViews;
	std::vector<ComPtr<ID3D11ShaderResourceView>> m_outputTextures;

	ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	ComPtr<ID3D11ShaderResourceView> m_depthOutputTexture;
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
