#pragma once
#include "library/CommonTypes.h"
#include "library/NonCopyable.hpp"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	class Application;

	class FullScreenRenderTarget : public NonCopyable<FullScreenRenderTarget>
	{
	public:
		explicit FullScreenRenderTarget(const Application& app);
		~FullScreenRenderTarget();

		ID3D11RenderTargetView* GetRenderTargetView() const { return m_renderTargetView.Get(); }
		ID3D11DepthStencilView* GetDepthStencilView() const { return m_depthStencilView.Get(); }
		ID3D11ShaderResourceView* GetOutputTexture() const { return m_outputTexture.Get(); }

		void Begin();
		void End();

	private:
		const Application& m_app;

		ComPtr<ID3D11RenderTargetView> m_renderTargetView;
		ComPtr<ID3D11DepthStencilView> m_depthStencilView;
		ComPtr<ID3D11ShaderResourceView> m_outputTexture;
	};
} // namespace library
