#include "StdAfx.h"
#include "library/Effect.h"

#include "library/Application.h"
#include "library/Path.h"
#include "library/Utils.h"
#include "library/Exception.h"

namespace library
{
	Effect::Effect(const Application& app)
		: m_app(app)
	{
	}

	ComPtr<ID3DX11Effect> Effect::CompileFromFile(ID3D11Device* const d3dDevice, const filesystem::Path& path)
	{
		ComPtr<ID3DX11Effect> effect;

		ComPtr<ID3DBlob> errorBlob;
		ComPtr<ID3DBlob> shaderBlob;

		auto hr = D3DCompileFromFile(
			path.GetWideCString(),
			nullptr,
			nullptr,
			nullptr,
			"fx_5_0",
#if defined(DEBUG) || defined(DEBUG)
			D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
#else
			0,
#endif
			0,
			shaderBlob.GetAddressOf(),
			errorBlob.GetAddressOf()
		);
		if (FAILED(hr))
		{
			std::string error = std::string("D3DX11CompileEffectFromFile() failed: ")
				+ std::string(static_cast<const char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize());
			throw Exception(error.c_str(), hr);
		}

		hr = D3DX11CreateEffectFromMemory(
			shaderBlob->GetBufferPointer(), shaderBlob->GetBufferSize(),
			0,
			d3dDevice,
			effect.GetAddressOf()
		);
		if (FAILED(hr))
		{
			throw Exception("D3DX11CreateEffectFromMemory() failed.", hr);
		}
	}

	void Effect::LoadCompiledEffect(ID3D11Device* const d3dDevice, const filesystem::Path& path)
	{

	}

	void Effect::SetEffect(const ComPtr<ID3DX11Effect>& effect)
	{
		if (m_effect != effect)
		{
			m_effect = effect;
		}
	}

	void Effect::Initialize()
	{

	}

} // namespace library
