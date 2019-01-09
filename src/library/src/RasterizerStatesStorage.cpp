#include "StdAfx.h"
#include "library/RasterizerStatesStorage.h"

#include "library/Exception.h"

#include <cassert>
#include <d3d11.h>

namespace library
{
	void RasterizerStatesStorage::Initialize(ID3D11Device* const device)
	{
		assert(!!device);

		// Back Culling
		{
			D3D11_RASTERIZER_DESC rasterizerStateDesc{};
			rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
			rasterizerStateDesc.DepthClipEnable = true;

			auto hr = device->CreateRasterizerState(&rasterizerStateDesc, s_states.at(RasterizerState::BackCulling).GetAddressOf());
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

			auto hr = device->CreateRasterizerState(&rasterizerStateDesc, s_states.at(RasterizerState::FrontCulling).GetAddressOf());
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

			auto hr = device->CreateRasterizerState(&rasterizerStateDesc, s_states.at(RasterizerState::DisabledCulling).GetAddressOf());
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

			auto hr = device->CreateRasterizerState(&rasterizerStateDesc, s_states.at(RasterizerState::Wireframe).GetAddressOf());
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateRasterizerState() failed.", hr);
			}
		}
	}

	void RasterizerStatesStorage::Reset()
	{
		s_states.clear();
	}

	ID3D11RasterizerState* RasterizerStatesStorage::GetState(const RasterizerState rs)
	{
		return s_states.at(rs).Get();
	}

} // namespace library
