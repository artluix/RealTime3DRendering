#include "StdAfx.h"
#include "library/Effect/EffectPass.h"

#include "library/Effect/EffectTechnique.h"
#include "library/Application.h"

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

EffectPass::~EffectPass() = default;

ComPtr<ID3D11InputLayout> EffectPass::CreateInputLayout(const InputElementDescArrayBuffer& descriptionsBuffer)
{
	ComPtr<ID3D11InputLayout> inputLayout;

	auto hr = m_app.GetDevice()->CreateInputLayout(
		descriptionsBuffer.data,
		static_cast<UINT>(descriptionsBuffer.size),
		m_passDesc.pIAInputSignature,
		m_passDesc.IAInputSignatureSize,
		&inputLayout
		);
	assert("ID3D11Device::CreateInputLayout() failed." && SUCCEEDED(hr));

	return inputLayout;
}

void EffectPass::Apply(const unsigned flags, ID3D11DeviceContext* const deviceContext)
{
	m_pass->Apply(flags, deviceContext);
}
} // namespace library
