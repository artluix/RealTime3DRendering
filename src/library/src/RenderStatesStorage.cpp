#include "StdAfx.h"
#include "library/RenderStatesStorage.h"

#include "library/Application.h"

namespace library
{
	namespace
	{
		constexpr unsigned k_defaultSampleMask = -1;
		constexpr unsigned k_defaultStencilRef = -1;
	}

	RenderStatesStorage::RenderStatesStorage(const Application& app)
		: m_app(app)
		, m_sampleMask(k_defaultSampleMask)
		, m_stencilRef(k_defaultStencilRef)
	{
	}

	void RenderStatesStorage::Reset(const RenderState rs /*= RenderState::All*/)
	{
		auto deviceContext = m_app.GetD3DDeviceContext();

		switch (rs)
		{
		case RenderState::Rasterizer:
			deviceContext->RSSetState(nullptr);
			m_rasterizerState.Reset();
			break;

		case RenderState::Blend:
			deviceContext->OMSetBlendState(nullptr, nullptr, k_defaultSampleMask);
			m_blendState.Reset();
			break;

		case RenderState::DepthStencil:
			deviceContext->OMSetDepthStencilState(nullptr, k_defaultStencilRef);
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

	void RenderStatesStorage::SaveState(const RenderState rs /*= RenderState::All*/)
	{
		auto deviceContext = m_app.GetD3DDeviceContext();

		switch (rs)
		{
		case RenderState::Rasterizer:
			m_rasterizerState.Reset();
			deviceContext->RSGetState(m_rasterizerState.GetAddressOf());
			break;

		case RenderState::Blend:
			m_blendState.Reset();
			deviceContext->OMGetBlendState(m_blendState.GetAddressOf(), m_blendFactor.data(), &m_sampleMask);
			break;

		case RenderState::DepthStencil:
			m_depthStencilState.Reset();
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

	void RenderStatesStorage::RestoreState(const RenderState rs /*= RenderState::All*/)
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
