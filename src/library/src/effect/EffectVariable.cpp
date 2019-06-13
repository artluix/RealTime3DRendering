#include "StdAfx.h"
#include "library/Effect/EffectVariable.h"

namespace library
{
EffectVariable::EffectVariable(const Effect& effect, ID3DX11EffectVariable* const variable)
	: m_effect(effect)
	, m_variable(variable)
	, m_type(variable->GetType())
{
	m_variable->GetDesc(&m_variableDesc);
	m_type->GetDesc(&m_typeDesc);
	m_name = m_variableDesc.Name;
}

EffectVariable::~EffectVariable() = default;

//-------------------------------------------------------------------------

ID3DX11EffectScalarVariable* EffectVariable::ToScalarVariable()
{
	auto variable = m_variable->AsScalar();
	if (variable->IsValid())
		return variable;

	assert("Invalid effect variable cast to scalar." && false);
	return nullptr;
}

ID3DX11EffectVectorVariable* EffectVariable::ToVectorVariable()
{
	auto variable = m_variable->AsVector();
	if (variable->IsValid())
		return variable;

	assert("Invalid effect variable cast to vector." && false);
	return nullptr;
}

ID3DX11EffectMatrixVariable* EffectVariable::ToMatrixVariable()
{
	auto variable = m_variable->AsMatrix();
	if (variable->IsValid())
		return variable;

	assert("Invalid effect variable cast to matrix." && false);
	return nullptr;
}

ID3DX11EffectShaderResourceVariable* EffectVariable::ToShaderResourceVariable()
{
	auto variable = m_variable->AsShaderResource();
	if (variable->IsValid())
		return variable;

	assert("Invalid effect variable cast to shader resource." && false);
	return nullptr;
}

//-------------------------------------------------------------------------

EffectVariable& EffectVariable::operator<<(ID3D11ShaderResourceView* const value)
{
	if (auto shaderResourceVariable = ToShaderResourceVariable())
		shaderResourceVariable->SetResource(value);

	return *this;
}

//-------------------------------------------------------------------------

void EffectVariable::SetMatrix(ID3DX11EffectMatrixVariable* const matrixVariable, const float* data)
{
	matrixVariable->SetMatrix(data);
}

void EffectVariable::SetMatrixArray(
	ID3DX11EffectMatrixVariable* const matrixVariable,
	const float* data, const std::size_t count
)
{
	matrixVariable->SetMatrixArray(data, 0, static_cast<unsigned>(count));
}
} // namespace library
