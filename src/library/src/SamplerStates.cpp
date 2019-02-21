#include "StdAfx.h"
#include "library/SamplerStates.h"

#include "library/Utils.h"
#include "library/Exception.h"
#include "library/Color.h"

#include <cassert>
#include <algorithm>
#include <d3d11_1.h>

namespace library
{
	namespace
	{
		void CreateSamplerState(ID3D11Device* const device, const D3D11_SAMPLER_DESC& samplerStateDesc, ID3D11SamplerState* samplerState)
		{
			auto hr = device->CreateSamplerState(&samplerStateDesc, &samplerState);
			if (FAILED(hr))
			{
				throw Exception("ID3D11Device::CreateSamplerState() failed.", hr);
			}
		}
	}

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
			const auto borderColor = Color(0.f, 0.f, 0.f, 1.f);

			D3D11_SAMPLER_DESC samplerStateDesc{};
			samplerStateDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerStateDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerStateDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
			samplerStateDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

			std::copy(borderColor.data.begin(), borderColor.data.end(), std::begin(samplerStateDesc.BorderColor));

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
