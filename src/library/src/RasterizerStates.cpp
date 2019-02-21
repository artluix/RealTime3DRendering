#include "StdAfx.h"
#include "library/RasterizerStates.h"

#include "library/Utils.h"
#include "library/Exception.h"

#include <cassert>
#include <d3d11_1.h>

namespace library
{
	namespace
	{
		void CreateRasterizerState(ID3D11Device* const device, const D3D11_RASTERIZER_DESC& rasterizerStateDesc, ID3D11RasterizerState* rasterizerState)
		{
			auto hr = device->CreateRasterizerState(&rasterizerStateDesc, &rasterizerState);
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateRasterizerState() failed.", hr);
			}
		}
	}

	//-------------------------------------------------------------------------

	ID3D11RasterizerState* RasterizerStates::BackCulling = nullptr;
	ID3D11RasterizerState* RasterizerStates::FrontCulling = nullptr;
	ID3D11RasterizerState* RasterizerStates::DisabledCulling = nullptr;
	ID3D11RasterizerState* RasterizerStates::Wireframe = nullptr;

	//-------------------------------------------------------------------------

	void RasterizerStates::Initialize(ID3D11Device* const device)
	{
		assert(!!device);

		// Back Culling
		{
			D3D11_RASTERIZER_DESC rasterizerStateDesc{};
			rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
			rasterizerStateDesc.DepthClipEnable = true;

			CreateRasterizerState(device, rasterizerStateDesc, BackCulling);
		}

		// Front Culling
		{
			D3D11_RASTERIZER_DESC rasterizerStateDesc{};
			rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerStateDesc.CullMode = D3D11_CULL_BACK;
			rasterizerStateDesc.FrontCounterClockwise = true;
			rasterizerStateDesc.DepthClipEnable = true;

			CreateRasterizerState(device, rasterizerStateDesc, FrontCulling);
		}

		// Disabled Culling
		{
			D3D11_RASTERIZER_DESC rasterizerStateDesc{};
			rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;
			rasterizerStateDesc.CullMode = D3D11_CULL_NONE;
			rasterizerStateDesc.DepthClipEnable = true;

			CreateRasterizerState(device, rasterizerStateDesc, DisabledCulling);
		}

		// Wireframe
		{
			D3D11_RASTERIZER_DESC rasterizerStateDesc{};
			rasterizerStateDesc.FillMode = D3D11_FILL_WIREFRAME;
			rasterizerStateDesc.CullMode = D3D11_CULL_NONE;
			rasterizerStateDesc.DepthClipEnable = true;

			CreateRasterizerState(device, rasterizerStateDesc, Wireframe);
		}
	}

	void RasterizerStates::Clear()
	{
		Release(Wireframe);
		Release(DisabledCulling);
		Release(FrontCulling);
		Release(BackCulling);
	}
} // namespace library
