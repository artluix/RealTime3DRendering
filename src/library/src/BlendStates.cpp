#include "StdAfx.h"
#include "library/BlendStates.h"

#include "library/Utils.h"

#include <cassert>
#include <d3d11_1.h>

namespace library
{

namespace
{
void CreateBlendState(
	ID3D11Device* const device,
	const D3D11_BLEND_DESC& blendStateDesc,
	ID3D11BlendState*& blendState)
{
	auto hr = device->CreateBlendState(&blendStateDesc, &blendState);
	assert("ID3D11Device::CreateBlendState() failed." && SUCCEEDED(hr));
}
} // namespace

//-------------------------------------------------------------------------

ID3D11BlendState* BlendStates::Alpha = nullptr;
ID3D11BlendState* BlendStates::Multiplicative = nullptr;

//-------------------------------------------------------------------------

void BlendStates::Initialize(ID3D11Device* const device)
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

		CreateBlendState(device, blendStateDesc, Alpha);
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

		CreateBlendState(device, blendStateDesc, Multiplicative);
	}
}

void BlendStates::Clear()
{
	Release(Multiplicative);
	Release(Alpha);
}
} // namespace library
