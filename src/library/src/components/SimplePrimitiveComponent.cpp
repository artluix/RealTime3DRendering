#include "StdAfx.h"
#include "library/Components/SimplePrimitiveComponent.h"

#include "library/Path.h"
#include "library/Utils.h"

namespace library
{
SimplePrimitiveComponent::SimplePrimitiveComponent() = default;
SimplePrimitiveComponent::~SimplePrimitiveComponent() = default;

//-------------------------------------------------------------------------

void SimplePrimitiveComponent::InitializeEffect(const std::string& effectName, const bool compile /*= false*/)
{
	ComPtr<ID3DBlob> errorBlob;
	ComPtr<ID3DBlob> shaderBlob;

	if (compile)
	{
		const auto path = GetApp().GetEffectsPath() + Path(effectName + ".fx");

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
			&shaderBlob,
			&errorBlob);
		if (FAILED(hr))
		{
			std::string error =
				std::string("D3DX11CompileEffectFromFile() failed: ") +
				std::string(static_cast<const char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize());
			throw Exception(error.c_str(), hr);
		}

		hr = D3DX11CreateEffectFromMemory(
			shaderBlob->GetBufferPointer(),
			shaderBlob->GetBufferSize(),
			0,
			GetApp().GetDevice(),
			&m_effect);
		if (FAILED(hr))
			throw Exception("D3DX11CreateEffectFromMemory() failed.", hr);
	}
	else
	{
		const auto path = GetApp().GetEffectsPath() + Path(effectName + ".fxc");

		std::vector<std::byte> effectData;
		utils::LoadBinaryFile(path, effectData);
		if (effectData.empty())
			throw Exception("Load compiled effect failed.");

		auto hr =
			D3DX11CreateEffectFromMemory(effectData.data(), effectData.size(), 0, GetApp().GetDevice(), &m_effect);
		if (FAILED(hr))
			throw Exception("D3DX11CreateEffectFromMemory() failed.", hr);
	}
}

//-------------------------------------------------------------------------

void SimplePrimitiveComponent::Draw_SetData(const PrimitiveData& primitiveData)
{
	m_pass->Apply(0, GetApp().GetDeviceContext());
}
} // namespace library
