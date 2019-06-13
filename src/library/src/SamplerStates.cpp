#include "StdAfx.h"
#include "library/SamplerStates.h"

#include "library/Utils.h"
#include "library/math/Color.h"

#include <d3d11_1.h>
#include <cassert>
#include <cstring>

namespace library
{

namespace
{
void CreateSamplerState(
	ID3D11Device* const device,
	const D3D11_SAMPLER_DESC& samplerStateDesc,
	ID3D11SamplerState*& samplerState)
{
	auto hr = device->CreateSamplerState(&samplerStateDesc, &samplerState);
	assert("ID3D11Device::CreateSamplerState() failed." && SUCCEEDED(hr));
}
} // namespace

//-------------------------------------------------------------------------

ID3D11SamplerState* SamplerStates::TrilinearWrap = nullptr;
ID3D11SamplerState* SamplerStates::TrilinearMirror = nullptr;
ID3D11SamplerState* SamplerStates::TrilinearClamp = nullptr;
ID3D11SamplerState* SamplerStates::TrilinearBorder = nullptr;

//-------------------------------------------------------------------------

void SamplerStates::Initialize(ID3D11Device* const device)
{
	assert(!!device);

	// Trilinear Wrap
	{
		D3D11_SAMPLER_DESC samplerStateDesc{};
		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

		CreateSamplerState(device, samplerStateDesc, TrilinearWrap);
	}

	// Trilinear Mirror
	{
		D3D11_SAMPLER_DESC samplerStateDesc{};
		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;

		CreateSamplerState(device, samplerStateDesc, TrilinearMirror);
	}

	// Trilinear Clamp
	{
		D3D11_SAMPLER_DESC samplerStateDesc{};
		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

		CreateSamplerState(device, samplerStateDesc, TrilinearClamp);
	}

	// Trilinear Border
	{
		constexpr auto borderColor = math::Color(0.f, 0.f, 0.f, 1.f);

		D3D11_SAMPLER_DESC samplerStateDesc{};
		samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

		memcpy(samplerStateDesc.BorderColor, static_cast<const float*>(borderColor), sizeof(borderColor));

		CreateSamplerState(device, samplerStateDesc, TrilinearBorder);
	}
}

void SamplerStates::Clear()
{
	Release(TrilinearBorder);
	Release(TrilinearClamp);
	Release(TrilinearMirror);
	Release(TrilinearWrap);
}
} // namespace library
