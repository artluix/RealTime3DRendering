#include "StdAfx.h"
#include "library/RenderStatesStorage.h"

#include <cassert>
#include <d3d11.h>

namespace library
{
	namespace
	{
		constexpr unsigned k_defaultSampleMask = -1;
		constexpr unsigned k_defaultStencilRef = -1;
	}

	//-------------------------------------------------------------------------

	ID3D11DeviceContext* RenderStatesStorage::s_deviceContext = nullptr;

	ComPtr<ID3D11RasterizerState> RenderStatesStorage::s_rasterizerState;
	ComPtr<ID3D11BlendState> RenderStatesStorage::s_blendState;
	ComPtr<ID3D11DepthStencilState> RenderStatesStorage::s_depthStencilState;

	std::array<float, 4> RenderStatesStorage::s_blendFactor;
	unsigned RenderStatesStorage::s_sampleMask = k_defaultSampleMask;
	unsigned RenderStatesStorage::s_stencilRef = k_defaultStencilRef;

	//-------------------------------------------------------------------------

	void RenderStatesStorage::SetDeviceContext(ID3D11DeviceContext* const deviceContext)
	{
		if (s_deviceContext != deviceContext)
		{
			Reset();

			s_deviceContext = deviceContext;
		}
	}

	void RenderStatesStorage::ResetState(const RenderState rs /*= RenderState::All*/)
	{
		assert(!!s_deviceContext);

		switch (rs)
		{
		case RenderState::Rasterizer:
			s_deviceContext->RSSetState(nullptr);
			s_rasterizerState.Reset();
			break;

		case RenderState::Blend:
			s_deviceContext->OMSetBlendState(nullptr, nullptr, k_defaultSampleMask);
			s_blendState.Reset();
			break;

		case RenderState::DepthStencil:
			s_deviceContext->OMSetDepthStencilState(nullptr, k_defaultStencilRef);
			s_depthStencilState.Reset();
			break;

		case RenderState::All:
			ResetState(RenderState::Rasterizer);
			ResetState(RenderState::Blend);
			ResetState(RenderState::DepthStencil);
			break;

		default:
			break;
		}
	}

	void RenderStatesStorage::SaveState(const RenderState rs /*= RenderState::All*/)
	{
		assert(!!s_deviceContext);

		switch (rs)
		{
		case RenderState::Rasterizer:
			s_rasterizerState.Reset();
			s_deviceContext->RSGetState(s_rasterizerState.GetAddressOf());
			break;

		case RenderState::Blend:
			s_blendState.Reset();
			s_deviceContext->OMGetBlendState(s_blendState.GetAddressOf(), s_blendFactor.data(), &s_sampleMask);
			break;

		case RenderState::DepthStencil:
			s_depthStencilState.Reset();
			s_deviceContext->OMGetDepthStencilState(s_depthStencilState.GetAddressOf(), &s_stencilRef);
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
		assert(!!s_deviceContext);

		switch (rs)
		{
		case RenderState::Rasterizer:
			s_deviceContext->RSSetState(s_rasterizerState.Get());
			break;

		case RenderState::Blend:
			s_deviceContext->OMSetBlendState(s_blendState.Get(), s_blendFactor.data(), s_sampleMask);
			break;

		case RenderState::DepthStencil:
			s_deviceContext->OMSetDepthStencilState(s_depthStencilState.Get(), s_stencilRef);
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

	void RenderStatesStorage::Reset()
	{
		s_deviceContext = nullptr;

		s_rasterizerState.Reset();
		s_blendState.Reset();
		s_depthStencilState.Reset();

		s_sampleMask = k_defaultSampleMask;
		s_stencilRef = k_defaultStencilRef;

		s_blendFactor.fill(0.f);
	}

} // namespace library
