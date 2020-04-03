#pragma once
#include "library/Common.h"
#include "library/NonCopyable.h"
#include "library/Math/Vector.h"
#include "library/Math/Matrix.h"

#include <d3dx11effect.h>
#include <string>

interface ID3D11ShaderResourceView;
interface ID3D11UnorderedAccessView;

namespace library
{

template<typename T>
inline constexpr bool is_hlsl_scalar =
	std::is_same_v<T, int> ||
	std::is_same_v<T, unsigned> ||
	std::is_same_v<T, bool> ||
	std::is_same_v<T, float> ||
	std::is_same_v<T, double>;

//-------------------------------------------------------------------------

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

	// raw
	template<typename T>
	std::enable_if_t<!is_hlsl_scalar<T>, EffectVariable&>
	operator<<(const T& value);

	template<typename T>
	std::enable_if_t<!is_hlsl_scalar<T>, EffectVariable&>
	operator<<(const DynArray<T>& value);

	template <typename T, std::size_t Count>
	std::enable_if_t<!is_hlsl_scalar<T>, EffectVariable&>
	operator<<(const Array<T, Count>& value);

	// scalar
	template<typename T>
	std::enable_if_t<is_hlsl_scalar<T>, EffectVariable&>
	operator<<(const T value);

	template<typename T>
	std::enable_if_t<is_hlsl_scalar<T>, EffectVariable&>
	operator<<(const DynArray<T>& value);

	template <typename T, std::size_t Count>
	std::enable_if_t<is_hlsl_scalar<T>, EffectVariable&>
	operator<<(const Array<T, Count>& value);

	// vector
	template<typename T, unsigned Size>
	EffectVariable& operator<<(const math::VectorType<T, Size>& value);
	template<typename T, unsigned Size>
	EffectVariable& operator<<(const math::VectorDynArray<T, Size>& value);
	template <typename T, unsigned Size, std::size_t Count>
	EffectVariable& operator<<(const math::VectorArray<T, Size, Count>& value);

	// matrix
	template<unsigned Size>
	EffectVariable& operator<<(const math::Matrix<Size>& value);
	template<unsigned Size>
	EffectVariable& operator<<(const math::MatrixDynArray<Size>& value);
	template <unsigned Size, std::size_t Count>
	EffectVariable& operator<<(const math::MatrixArray<Size, Count>& value);

	// textures
	EffectVariable& operator<<(ID3D11ShaderResourceView* const value);
	EffectVariable& operator<<(ID3D11UnorderedAccessView* const value);

private:
	ID3DX11EffectScalarVariable* ToScalarVariable();
	ID3DX11EffectVectorVariable* ToVectorVariable();
	ID3DX11EffectMatrixVariable* ToMatrixVariable();
	ID3DX11EffectShaderResourceVariable* ToShaderResourceVariable();
	ID3DX11EffectUnorderedAccessViewVariable* ToUAVVariable();

	//-------------------------------------------------------------------------

	template<typename T>
	void SetRaw(ID3DX11EffectVariable* const variable, const T& value);

	template<typename T>
	void SetRawArray(ID3DX11EffectVariable* const variable, const T* data, const std::size_t count);

	template<typename T>
	void SetScalar(ID3DX11EffectScalarVariable* const scalarVariable, const T value);
	template<typename T>
	void SetScalarArray(
		ID3DX11EffectScalarVariable* const scalarVariable,
		const T* data, const std::size_t count
	);

	template<typename T>
	void SetVector(ID3DX11EffectVectorVariable* const vectorVariable, const T* data);
	template<typename T>
	void SetVectorArray(
		ID3DX11EffectVectorVariable* const vectorVariable,
		const T* data,
		const std::size_t count
	);

	void SetMatrix(ID3DX11EffectMatrixVariable* const matrixVariable, const float* data);
	void SetMatrixArray(
		ID3DX11EffectMatrixVariable* const matrixVariable,
		const float* data, const std::size_t count
	);

	//-------------------------------------------------------------------------

	const Effect& m_effect;
	std::string m_name;

	ComPtr<ID3DX11EffectVariable> m_variable;
	D3DX11_EFFECT_VARIABLE_DESC m_variableDesc;

	ComPtr<ID3DX11EffectType> m_type;
	D3DX11_EFFECT_TYPE_DESC m_typeDesc;
};
} // namespace library

#include "library/Effect/EffectVariable-inl.h"
