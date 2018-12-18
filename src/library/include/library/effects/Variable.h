#pragma once
#include "library/Common.h"
#include "library/NonCopyable.hpp"
#include "library/Math.h"

#include <d3dx11effect.h>
#include <string>

interface ID3D11ShaderResourceView;

namespace library
{
	namespace effects
	{
		class Effect;

		class Variable : public NonCopyable<Variable>
		{
		public:
			explicit Variable(Effect& effect, const ComPtr<ID3DX11EffectVariable>& variable);

			Effect& GetEffect() { return m_effect; }
			const std::string& GetName() const { return m_name; }

			ID3DX11EffectVariable* const GetVariable() const { return m_variable.Get(); }
			const D3DX11_EFFECT_VARIABLE_DESC& GetVariableDesc() const { return m_variableDesc; }

			ID3DX11EffectType* const GetType() const { return m_type.Get(); }
			const D3DX11_EFFECT_TYPE_DESC& GetTypeDesc() const { return m_typeDesc; }

			Variable& operator << (const float value);
			Variable& operator << (ID3D11ShaderResourceView* const value);

			// vector
			Variable& operator << (const math::XMVector& value);

			template<std::size_t Size>
			Variable& operator << (const math::Vector<Size>& value);

			// matrix
			Variable& operator << (const math::XMMatrix& value);

			template<std::size_t Size>
			Variable& operator << (const math::Matrix<Size>& value);

		private:
			Effect& m_effect;
			std::string m_name;

			ComPtr<ID3DX11EffectVariable> m_variable;
			D3DX11_EFFECT_VARIABLE_DESC m_variableDesc;

			ComPtr<ID3DX11EffectType> m_type;
			D3DX11_EFFECT_TYPE_DESC m_typeDesc;
		};

		using VariablePtr = std::shared_ptr<Variable>;

		// ----------------------------------------------------------------------------------------------------------

		template<std::size_t Size>
		inline Variable& Variable::operator << (const math::Matrix<Size>& value)
		{
			this << value.Load();
			return *this;
		}

		template<std::size_t Size>
		inline Variable& Variable::operator<<(const math::Vector<Size>& value)
		{
			this << value.Load();
			return *this;
		}

	} // namespace effects
} // namespace library
