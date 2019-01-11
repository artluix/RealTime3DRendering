#include "StdAfx.h"
#include "library/RasterizerStateHolder.h"

#include "library/Exception.h"

#include <cassert>
#include <d3d11.h>

namespace library
{
	std::map<library::RasterizerState, ComPtr<ID3D11RasterizerState>> RasterizerStateHolder::s_rasterizerStates;

	void RasterizerStateHolder::Initialize(ID3D11Device* const device)
	{
		assert(!!device);

		// Back Culling
		{
			D3D11_RASTERIZER_DESC rasterizerStateDesc{};
			rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
			rasterizerStateDesc.DepthClipEnable = true;

			auto hr = device->CreateRasterizerState(&rasterizerStateDesc, s_rasterizerStates.at(RasterizerState::BackCulling).GetAddressOf());
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

			auto hr = device->CreateRasterizerState(&rasterizerStateDesc, s_rasterizerStates.at(RasterizerState::FrontCulling).GetAddressOf());
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

			auto hr = device->CreateRasterizerState(&rasterizerStateDesc, s_rasterizerStates.at(RasterizerState::DisabledCulling).GetAddressOf());
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

			auto hr = device->CreateRasterizerState(&rasterizerStateDesc, s_rasterizerStates.at(RasterizerState::Wireframe).GetAddressOf());
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateRasterizerState() failed.", hr);
			}
		}
	}

	void RasterizerStateHolder::Reset()
	{
		s_rasterizerStates.clear();
	}

	ID3D11RasterizerState* RasterizerStateHolder::GetRasterizerState(const RasterizerState rs)
	{
		return s_rasterizerStates.at(rs).Get();
	}
} // namespace library
