#pragma once
#include "library/CommonTypes.h"
#include "library/NonCopyable.hpp"
#include "library/math/Math.h"
#include "library/Exception.h"

#include <d3dx11effect.h>
#include <string>
#include <vector>

interface ID3D11ShaderResourceView;

namespace library
{
	class Effect;

	class EffectVariable : public NonCopyable<EffectVariable>
	{
	public:
		explicit EffectVariable(const Effect& effect, ID3DX11EffectVariable* const variable);
		~EffectVariable();

		const Effect& GetEffect() const { return m_effect; }
		const std::string& GetName() const { return m_name; }

		ID3DX11EffectVariable* GetVariable() const { return m_variable.Get(); }
		const D3DX11_EFFECT_VARIABLE_DESC& GetVariableDesc() const { return m_variableDesc; }

		ID3DX11EffectType* GetType() const { return m_type.Get(); }
		const D3DX11_EFFECT_TYPE_DESC& GetTypeDesc() const { return m_typeDesc; }

		EffectVariable& operator << (const float value);
		EffectVariable& operator << (ID3D11ShaderResourceView* const value);

		// vector
		EffectVariable& operator << (const math::XMVector& value);

		template<std::size_t Size>
		EffectVariable& operator << (const math::Vector<Size>& value);

		// matrix
		EffectVariable& operator << (const math::XMMatrix& value);

		template<std::size_t Size>
		EffectVariable& operator << (const math::Matrix<Size>& value);

		// vector of types
		EffectVariable& operator << (const std::vector<float>& value);

		template<std::size_t Size>
		EffectVariable& operator << (const std::vector<math::Vector<Size>>& value);

		template<std::size_t Size>
		EffectVariable& operator << (const std::vector<math::Matrix<Size>>& value);

	private:
		const Effect& m_effect;
		std::string m_name;

		ComPtr<ID3DX11EffectVariable> m_variable;
		D3DX11_EFFECT_VARIABLE_DESC m_variableDesc;

		ComPtr<ID3DX11EffectType> m_type;
		D3DX11_EFFECT_TYPE_DESC m_typeDesc;
	};

	// ----------------------------------------------------------------------------------------------------------

	template<std::size_t Size>
	inline EffectVariable& EffectVariable::operator << (const math::Vector<Size>& value)
	{
		auto variable = m_variable->AsVector();
		if (!variable->IsValid())
		{
			throw Exception("Invalid effect variable cast.");
		}

		variable->SetFloatVector(static_cast<const float*>(value));
		return *this;
	}

	template<std::size_t Size>
	inline EffectVariable& EffectVariable::operator << (const math::Matrix<Size>& value)
	{
		auto variable = m_variable->AsMatrix();
		if (!variable->IsValid())
		{
			throw Exception("Invalid effect variable cast.");
		}

		variable->SetMatrix(static_cast<const float*>(value));
		return *this;
	}

	template<std::size_t Size>
	inline EffectVariable& EffectVariable::operator << (const std::vector<math::Vector<Size>>& value)
	{
		auto variable = m_variable->AsVector();
		if (!variable->IsValid())
		{
			throw Exception("Invalid effect variable cast.");
		}

		variable->SetFloatVectorArray(reinterpret_cast<const float*>(value.data()), 0, value.size());
		return *this;
	}

	template<std::size_t Size>
	inline EffectVariable& EffectVariable::operator << (const std::vector<math::Matrix<Size>>& value)
	{
		auto variable = m_variable->AsMatrix();
		if (!variable->IsValid())
		{
			throw Exception("Invalid effect variable cast.");
		}

		variable->SetMatrixArray(reinterpret_cast<const float*>(value.data()), 0, value.size());
		return *this;
	}
} // namespace library
