#pragma once
#include "library/Common.h"
#include "library/NonCopyable.hpp"
#include "library/math/Math.h"

#include <d3dx11effect.h>
#include <string>

interface ID3D11ShaderResourceView;

namespace library
{
	namespace effect
	{
		class Effect;

		class Variable : public NonCopyable<Variable>
		{
		public:
			explicit Variable(const Effect& effect, ID3DX11EffectVariable* const variable);
			~Variable();

			const Effect& GetEffect() const { return m_effect; }
			const std::string& GetName() const { return m_name; }

			ID3DX11EffectVariable* GetVariable() const { return m_variable.Get(); }
			const D3DX11_EFFECT_VARIABLE_DESC& GetVariableDesc() const { return m_variableDesc; }

			ID3DX11EffectType* GetType() const { return m_type.Get(); }
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
			const Effect& m_effect;
			std::string m_name;

			ComPtr<ID3DX11EffectVariable> m_variable;
			D3DX11_EFFECT_VARIABLE_DESC m_variableDesc;

			ComPtr<ID3DX11EffectType> m_type;
			D3DX11_EFFECT_TYPE_DESC m_typeDesc;
		};

		// ----------------------------------------------------------------------------------------------------------

		template<std::size_t Size>
		inline Variable& Variable::operator << (const math::Matrix<Size>& value)
		{
			auto& thisRef = *this;
			thisRef << value.Load();
			return thisRef;
		}

		template<std::size_t Size>
		inline Variable& Variable::operator<<(const math::Vector<Size>& value)
		{
			auto& thisRef = *this;
			thisRef << value.Load();
			return thisRef;
		}

	} // namespace effect
} // namespace library
