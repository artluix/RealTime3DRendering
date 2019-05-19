#include "StdAfx.h"
#include "library/Effect/EffectVariable.h"

#include "library/Exception.h"

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

EffectVariable& EffectVariable::operator<<(const float value)
{
	auto variable = m_variable->AsScalar();
	if (!variable->IsValid())
	{
		throw Exception("Invalid effect variable cast.");
	}

	variable->SetFloat(value);
	return *this;
}

EffectVariable& EffectVariable::operator<<(const std::vector<float>& value)
{
	SetScalarArray(value.data(), value.size());
	return *this;
}

//-------------------------------------------------------------------------

EffectVariable& EffectVariable::operator<<(ID3D11ShaderResourceView* const value)
{
	auto variable = m_variable->AsShaderResource();
	if (!variable->IsValid())
	{
		throw Exception("Invalid effect variable cast to shader resource.");
	}

	variable->SetResource(value);
	return *this;
}

//-------------------------------------------------------------------------

EffectVariable& EffectVariable::operator<<(const math::XMVector& value)
{
	auto variable = m_variable->AsVector();
	if (!variable->IsValid())
	{
		throw Exception("Invalid effect variable cast to vector.");
	}

	variable->SetFloatVector(reinterpret_cast<const float*>(&value));
	return *this;
}

EffectVariable& EffectVariable::operator<<(const std::vector<math::XMVector>& value)
{
	SetVectorArray(value.data(), value.size());
	return *this;
}

//-------------------------------------------------------------------------

EffectVariable& EffectVariable::operator<<(const math::XMMatrix& value)
{
	auto variable = m_variable->AsMatrix();
	if (!variable->IsValid())
	{
		throw Exception("Invalid effect variable cast to matrix.");
	}

	variable->SetMatrix(reinterpret_cast<const float*>(&value));
	return *this;
}

EffectVariable& EffectVariable::operator<<(const std::vector<math::XMMatrix>& value)
{
	SetMatrixArray(value.data(), value.size());
	return *this;
}

//-------------------------------------------------------------------------

void EffectVariable::SetScalarArray(const float* data, const std::size_t count)
{
	auto variable = m_variable->AsScalar();
	if (!variable->IsValid())
	{
		throw Exception("Invalid effect variable cast to scalar.");
	}

	variable->SetFloatArray(data, 0, static_cast<unsigned>(count));
}

void EffectVariable::SetVectorArray(const math::XMVector* data, const std::size_t count)
{
	auto variable = m_variable->AsVector();
	if (!variable->IsValid())
	{
		throw Exception("Invalid effect variable cast to vector.");
	}

	variable->SetFloatVectorArray(reinterpret_cast<const float*>(data), 0, static_cast<unsigned>(count));
}

void EffectVariable::SetMatrixArray(const math::XMMatrix* data, const std::size_t count)
{
	auto variable = m_variable->AsMatrix();
	if (!variable->IsValid())
	{
		throw Exception("Invalid effect variable cast to matrix.");
	}

	variable->SetMatrixArray(reinterpret_cast<const float*>(data), 0, static_cast<unsigned>(count));
}
} // namespace library
