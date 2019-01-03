#include "StdAfx.h"
#include "library/effects/EffectVariable.h"

#include "library/Exception.h"

namespace library
{
	EffectVariable::EffectVariable(const Effect& effect, const ComPtr<ID3DX11EffectVariable>& variable)
		: m_effect(effect)
		, m_variable(variable)
		, m_type(nullptr)
	{
		m_variable->GetDesc(&m_variableDesc);
		m_name = m_variableDesc.Name;
		
		m_type = m_variable->GetType();
		m_type->GetDesc(&m_typeDesc);
	}

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

	EffectVariable& EffectVariable::operator<<(ID3D11ShaderResourceView* const value)
	{
		auto variable = m_variable->AsShaderResource();
		if (!variable->IsValid())
		{
			throw Exception("Invalid effect variable cast.");
		}

		variable->SetResource(value);
		return *this;
	}

	EffectVariable& EffectVariable::operator<<(const math::XMVector& value)
	{
		auto variable = m_variable->AsVector();
		if (!variable->IsValid())
		{
			throw Exception("Invalid effect variable cast.");
		}

		variable->SetFloatVector(reinterpret_cast<const float*>(&value));
		return *this;
	}

	EffectVariable& EffectVariable::operator<<(const math::XMMatrix& value)
	{
		auto variable = m_variable->AsMatrix();
		if (!variable->IsValid())
		{
			throw Exception("Invalid effect variable cast.");
		}

		variable->SetMatrix(reinterpret_cast<const float*>(&value));
		return *this;
	}

} // namespace library
