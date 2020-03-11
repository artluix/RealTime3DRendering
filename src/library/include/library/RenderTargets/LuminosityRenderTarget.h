#pragma once
#include "library/RenderTargets/RenderTarget.h"
#include "library/Common.h"

namespace library
{
class Application;

class LuminosityRenderTarget : public RenderTarget
{
	RTTI_CLASS(LuminosityRenderTarget, RenderTarget)

public:
	explicit LuminosityRenderTarget(const Application& app);
	~LuminosityRenderTarget();

	ID3D11RenderTargetView* GetRenderTargetView() const { return m_renderTargetView.Get(); }
	ID3D11ShaderResourceView* GetOutputTexture() const { return m_outputTexture.Get(); }

	unsigned GetMipMapLevelsCount() const { m_mipLevelsCount; }

	void Clear(const ClearParams& cp) override;
	void Begin() override;
	void End() override;

private:
	const Application& m_app;

	unsigned m_mipLevelsCount;

	ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	ComPtr<ID3D11ShaderResourceView> m_outputTexture;
};
} // namespace library
