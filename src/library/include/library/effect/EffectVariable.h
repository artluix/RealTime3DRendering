#pragma once
#include "library/CommonTypes.h"
#include "library/Math/XmTypes.h"
#include "library/NonCopyable.hpp"

#include <array>
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
	EffectVariable(const Effect& effect, ID3DX11EffectVariable* const variable);
	~EffectVariable();

	const Effect& GetEffect() const { return m_effect; }
	const std::string& GetName() const { return m_name; }

	ID3DX11EffectVariable* GetVariable() const { return m_variable.Get(); }
	const D3DX11_EFFECT_VARIABLE_DESC& GetVariableDesc() const { return m_variableDesc; }

	ID3DX11EffectType* GetType() const { return m_type.Get(); }
	const D3DX11_EFFECT_TYPE_DESC& GetTypeDesc() const { return m_typeDesc; }

	// scalar
	EffectVariable& operator<<(const float value);
	EffectVariable& operator<<(const std::vector<float>& value);
	template <std::size_t Count>
	EffectVariable& operator<<(const std::array<float, Count>& value);

	// texture
	EffectVariable& operator<<(ID3D11ShaderResourceView* const value);

	// vector
	EffectVariable& operator<<(const math::XMVector& value);
	EffectVariable& operator<<(const std::vector<math::XMVector>& value);
	template <std::size_t Count>
	EffectVariable& operator<<(const std::array<math::XMVector, Count>& value);

	// matrix
	EffectVariable& operator<<(const math::XMMatrix& value);
	EffectVariable& operator<<(const std::vector<math::XMMatrix>& value);
	template <std::size_t Count>
	EffectVariable& operator<<(const std::array<math::XMMatrix, Count>& value);

private:
	void SetScalarArray(const float* data, const std::size_t count);
	void SetVectorArray(const math::XMVector* data, const std::size_t count);
	void SetMatrixArray(const math::XMMatrix* data, const std::size_t count);

	const Effect& m_effect;
	std::string m_name;

	ComPtr<ID3DX11EffectVariable> m_variable;
	D3DX11_EFFECT_VARIABLE_DESC m_variableDesc;

	ComPtr<ID3DX11EffectType> m_type;
	D3DX11_EFFECT_TYPE_DESC m_typeDesc;
};

// ----------------------------------------------------------------------------------------------------------

template <std::size_t Count>
EffectVariable& library::EffectVariable::operator<<(const std::array<float, Count>& value)
{
	SetScalarArray(value.data(), Count);
	return *this;
}

template <std::size_t Count>
EffectVariable& library::EffectVariable::operator<<(const std::array<math::XMVector, Count>& value)
{
	SetVectorArray(value.data(), Count);
	return *this;
}

template <std::size_t Count>
EffectVariable& library::EffectVariable::operator<<(const std::array<math::XMMatrix, Count>& value)
{
	SetMatrixArray(value.data(), Count);
	return *this;
}
} // namespace library
