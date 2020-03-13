#pragma once
#include "library/RenderTargets/SecondaryRenderTarget.h"
#include "library/Common.h"

namespace library
{
class LuminosityRenderTarget : public SecondaryRenderTarget
{
	RTTI_CLASS(LuminosityRenderTarget, SecondaryRenderTarget)

public:
	explicit LuminosityRenderTarget(const Application& app);
	~LuminosityRenderTarget();

	ID3D11ShaderResourceView* GetOutputTexture() const { return m_outputTexture.Get(); }
	unsigned GetMipMapLevelsCount() const { m_mipLevelsCount; }

	RenderTargetViewArray GetRenderTargetViews() const override final { return { m_renderTargetView.Get() }; }

private:
	unsigned m_mipLevelsCount;

	ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	ComPtr<ID3D11ShaderResourceView> m_outputTexture;
};
} // namespace library
