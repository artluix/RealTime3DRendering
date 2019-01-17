#include "StdAfx.h"
#include "library/BlendStateHolder.h"

#include "library/Exception.h"

#include <cassert>
#include <d3d11_1.h>

namespace library
{
	std::array<BlendState::Ptr, BlendState::Count> BlendStateHolder::s_blendStates;

	//-------------------------------------------------------------------------

	void BlendStateHolder::Initialize(ID3D11Device* const device)
	{
		assert(!!device);

		// Alpha
		{
			D3D11_BLEND_DESC blendStateDesc{};
			blendStateDesc.RenderTarget[0].BlendEnable = true;
			blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
			blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			auto hr = device->CreateBlendState(&blendStateDesc, s_blendStates[BlendState::Alpha].GetAddressOf());
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateBlendState() failed.", hr);
			}
		}

		// Multiplicative
		{
			D3D11_BLEND_DESC blendStateDesc{};
			blendStateDesc.RenderTarget[0].BlendEnable = true;
			blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
			blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
			blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
			blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			blendStateDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			auto hr = device->CreateBlendState(&blendStateDesc, s_blendStates[BlendState::Multiplicative].GetAddressOf());
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateBlendState() failed.", hr);
			}
		}
	}

	void BlendStateHolder::Reset()
	{
		std::for_each(s_blendStates.begin(), s_blendStates.end(), [](BlendState::Ptr& rsp) { rsp.Reset(); });
	}

	BlendState::Ptr BlendStateHolder::GetBlendState(const BlendState::Type rs)
	{
		return s_blendStates[rs];
	}
} // namespace library
