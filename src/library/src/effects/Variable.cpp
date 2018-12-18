#include "StdAfx.h"
#include "library/effects/Variable.h"

#include "library/Exception.h"

namespace library
{
	namespace effects
	{
		Variable::Variable(Effect& effect, const ComPtr<ID3DX11EffectVariable>& variable)
			: m_effect(effect)
			, m_variable(variable)
		{
		}

		Variable& Variable::operator<<(const float value)
		{
			auto variable = m_variable->AsScalar();
			if (!variable->IsValid())
			{
				throw Exception("Invalid effect variable cast.");
			}

			variable->SetFloat(value);
			return *this;
		}

		Variable& Variable::operator<<(ID3D11ShaderResourceView* const value)
		{
			auto variable = m_variable->AsShaderResource();
			if (!variable->IsValid())
			{
				throw Exception("Invalid effect variable cast.");
			}

			variable->SetResource(value);
			return *this;
		}

		Variable& Variable::operator<<(const math::XMVector& value)
		{
			auto variable = m_variable->AsVector();
			if (!variable->IsValid())
			{
				throw Exception("Invalid effect variable cast.");
			}

			variable->SetFloatVector(reinterpret_cast<const float*>(&value));
			return *this;
		}

		Variable& Variable::operator<<(const math::XMMatrix& value)
		{
			auto variable = m_variable->AsMatrix();
			if (!variable->IsValid())
			{
				throw Exception("Invalid effect variable cast.");
			}

			variable->SetMatrix(reinterpret_cast<const float*>(&value));
			return *this;
		}

	} // namespace effects
} // namespace library
