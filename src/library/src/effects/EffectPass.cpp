#include "StdAfx.h"
#include "library/effects/EffectPass.h"

#include "library/Application.h"
#include "library/Exception.h"
#include "library/effects/EffectTechnique.h"

namespace library
{
	EffectPass::EffectPass(const Application& app, const EffectTechnique& technique, ID3DX11EffectPass* const pass)
		: m_app(app)
		, m_technique(technique)
		, m_pass(pass)
	{
		m_pass->GetDesc(&m_passDesc);
		m_name = m_passDesc.Name;
	}

	ComPtr<ID3D11InputLayout> EffectPass::CreateInputLayout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElementDescriptions) const
	{
		ComPtr<ID3D11InputLayout> inputLayout;

		auto hr = m_app.GetD3DDevice()->CreateInputLayout(
			inputElementDescriptions.data(),
			inputElementDescriptions.size(),
			m_passDesc.pIAInputSignature,
			m_passDesc.IAInputSignatureSize,
			inputLayout.GetAddressOf()
		);
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateInputLayout() failed.", hr);
		}

		return inputLayout;
	}

	void EffectPass::Apply(const unsigned flags, ID3D11DeviceContext* const deviceContext)
	{
		m_pass->Apply(flags, deviceContext);
	}

} // namespace library
