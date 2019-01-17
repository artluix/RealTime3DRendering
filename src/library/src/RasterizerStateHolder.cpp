#include "StdAfx.h"
#include "library/RasterizerStateHolder.h"

#include "library/Exception.h"

#include <cassert>
#include <d3d11_1.h>

namespace library
{
	std::array<RasterizerState::Ptr, RasterizerState::Count> RasterizerStateHolder::s_rasterizerStates;

	//-------------------------------------------------------------------------

	void RasterizerStateHolder::Initialize(ID3D11Device* const device)
	{
		assert(!!device);

		// Back Culling
		{
			D3D11_RASTERIZER_DESC rasterizerStateDesc{};
			rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
			rasterizerStateDesc.DepthClipEnable = true;

			auto hr = device->CreateRasterizerState(&rasterizerStateDesc, s_rasterizerStates[RasterizerState::FrontCulling].GetAddressOf());
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateRasterizerState() failed.", hr);
			}
		}

		// Front Culling
		{
			D3D11_RASTERIZER_DESC rasterizerStateDesc{};
			rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
			rasterizerStateDesc.FrontCounterClockwise = true;
			rasterizerStateDesc.DepthClipEnable = true;

			auto hr = device->CreateRasterizerState(&rasterizerStateDesc,s_rasterizerStates[RasterizerState::FrontCulling].GetAddressOf());
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateRasterizerState() failed.", hr);
			}
		}

		// Disabled Culling
		{
			D3D11_RASTERIZER_DESC rasterizerStateDesc{};
			rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerStateDesc.CullMode = D3D11_CULL_NONE;
			rasterizerStateDesc.DepthClipEnable = true;

			auto hr = device->CreateRasterizerState(&rasterizerStateDesc, s_rasterizerStates[RasterizerState::DisabledCulling].GetAddressOf());
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateRasterizerState() failed.", hr);
			}
		}

		// Wireframe
		{
			D3D11_RASTERIZER_DESC rasterizerStateDesc{};
			rasterizerStateDesc.FillMode = D3D11_FILL_WIREFRAME;
			rasterizerStateDesc.CullMode = D3D11_CULL_NONE;
			rasterizerStateDesc.DepthClipEnable = true;

			auto hr = device->CreateRasterizerState(&rasterizerStateDesc, s_rasterizerStates[RasterizerState::Wireframe].GetAddressOf());
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateRasterizerState() failed.", hr);
			}
		}
	}

	void RasterizerStateHolder::Reset()
	{
		std::for_each(s_rasterizerStates.begin(), s_rasterizerStates.end(), [](RasterizerState::Ptr& rsp) { rsp.Reset(); });
	}

	RasterizerState::Ptr RasterizerStateHolder::GetRasterizerState(const RasterizerState::Type rs)
	{
		return s_rasterizerStates[rs];
	}
} // namespace library
