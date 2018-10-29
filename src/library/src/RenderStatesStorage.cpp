#include "library/RenderStatesStorage.h"

#include "library/Application.h"

#include <limits>

namespace library
{

	namespace defaults
	{
		constexpr unsigned k_sampleMask = -1;
		constexpr unsigned k_stencilRef = -1;
	} // namespace defaults

	RenderStateStorage::RenderStateStorage(const Application& app)
		: m_app(app)
		, m_sampleMask(defaults::k_sampleMask)
		, m_stencilRef(defaults::k_stencilRef)
	{
	}

	void RenderStateStorage::Reset(const RenderState rs /*= RenderState::All*/)
	{
		auto deviceContext = m_app.GetD3DDeviceContext();

		switch (rs)
		{
		case RenderState::Rasterizer:
			deviceContext->RSSetState(nullptr);
			m_rasterizerState.Reset();
			break;

		case RenderState::Blend:
			deviceContext->OMSetBlendState(nullptr, nullptr, defaults::k_sampleMask);
			m_blendState.Reset();
			break;

		case RenderState::DepthStencil:
			deviceContext->OMSetDepthStencilState(nullptr, defaults::k_stencilRef);
			m_depthStencilState.Reset();
			break;

		case RenderState::All:
			Reset(RenderState::Rasterizer);
			Reset(RenderState::Blend);
			Reset(RenderState::DepthStencil);
			break;

		default:
			break;
		}
	}

	void RenderStateStorage::SaveState(const RenderState rs /*= RenderState::All*/)
	{
		auto deviceContext = m_app.GetD3DDeviceContext();

		switch (rs)
		{
		case RenderState::Rasterizer:
			deviceContext->RSGetState(m_rasterizerState.GetAddressOf());
			break;

		case RenderState::Blend:
			deviceContext->OMGetBlendState(m_blendState.GetAddressOf(), m_blendFactor.data(), &m_sampleMask);
			break;

		case RenderState::DepthStencil:
			deviceContext->OMGetDepthStencilState(m_depthStencilState.GetAddressOf(), &m_stencilRef);
			break;

		case RenderState::All:
			SaveState(RenderState::Rasterizer);
			SaveState(RenderState::Blend);
			SaveState(RenderState::DepthStencil);
			break;

		default:
			break;
		}
	}

	void RenderStateStorage::RestoreState(const RenderState rs /*= RenderState::All*/)
	{
		auto deviceContext = m_app.GetD3DDeviceContext();

		switch (rs)
		{
		case RenderState::Rasterizer:
			deviceContext->RSSetState(m_rasterizerState.Get());
			break;

		case RenderState::Blend:
			deviceContext->OMSetBlendState(m_blendState.Get(), m_blendFactor.data(), m_sampleMask);
			break;

		case RenderState::DepthStencil:
			deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), m_stencilRef);
			break;

		case RenderState::All:
			RestoreState(RenderState::Rasterizer);
			RestoreState(RenderState::Blend);
			RestoreState(RenderState::DepthStencil);
			break;

		default:
			break;
		}
	}

} // namespace library
