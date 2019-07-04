#pragma once

namespace library
{
//-------------------------------------------------------------------------
// SetRaw<T> & SetRawArray<T>
//-------------------------------------------------------------------------

template<typename T>
inline void EffectVariable::SetRaw(ID3DX11EffectVariable* const variable, const T& value)
{
	variable->SetRawValue(&value, 0, sizeof(T));
}

template<typename T>
inline void EffectVariable::SetRawArray(
	ID3DX11EffectVariable* const variable,
	const T* data, const std::size_t count
)
{
	variable->SetRawValue(data, 0, sizeof(T) * count);
}


//-------------------------------------------------------------------------
// SetScalar<T>
//-------------------------------------------------------------------------

template<>
inline void EffectVariable::SetScalar<float>(
	ID3DX11EffectScalarVariable* const scalarVariable,
	const float value
)
{
	scalarVariable->SetFloat(value);
}

template<>
inline void EffectVariable::SetScalar<s32>(
	ID3DX11EffectScalarVariable* const scalarVariable,
	const s32 value
)
{
	scalarVariable->SetInt(value);
}

template<>
inline void EffectVariable::SetScalar<u32>(
	ID3DX11EffectScalarVariable* const scalarVariable,
	const u32 value
)
{
	scalarVariable->SetInt(value);
}

template<>
inline void EffectVariable::SetScalar<bool>(
	ID3DX11EffectScalarVariable* const scalarVariable,
	const bool value
)
{
	scalarVariable->SetBool(value);
}

//-------------------------------------------------------------------------
// SetScalarArray<T>
//-------------------------------------------------------------------------

template<>
inline void EffectVariable::SetScalarArray<float>(
	ID3DX11EffectScalarVariable* const scalarVariable,
	const float* data, const std::size_t count
)
{
	scalarVariable->SetFloatArray(data, 0, count);
}

template<>
inline void EffectVariable::SetScalarArray<s32>(
	ID3DX11EffectScalarVariable* const scalarVariable,
	const s32* data, const std::size_t count
)
{
	scalarVariable->SetIntArray(data, 0, count);
}

template<>
inline void EffectVariable::SetScalarArray<u32>(
	ID3DX11EffectScalarVariable* const scalarVariable,
	const u32* data, const std::size_t count
)
{
	scalarVariable->SetIntArray(reinterpret_cast<const int*>(data), 0, count);
}

template<>
inline void EffectVariable::SetScalarArray<bool>(
	ID3DX11EffectScalarVariable* const scalarVariable,
	const bool* data, const std::size_t count
)
{
	scalarVariable->SetBoolArray(data, 0, count);
}

template<typename T>
inline void EffectVariable::SetScalarArray(
	ID3DX11EffectScalarVariable* const scalarVariable,
	const T* data, const std::size_t count
)
{
	scalarVariable->SetRawValue(data, 0, sizeof(T) * count);
}

//-------------------------------------------------------------------------
// SetVector<T>
//-------------------------------------------------------------------------

template<>
inline void EffectVariable::SetVector<float>(ID3DX11EffectVectorVariable* const vectorVariable, const float* data)
{
	vectorVariable->SetFloatVector(data);
}

template<>
inline void EffectVariable::SetVector<s32>(ID3DX11EffectVectorVariable* const vectorVariable, const s32* data)
{
	vectorVariable->SetIntVector(data);
}

template<>
inline void EffectVariable::SetVector<u32>(ID3DX11EffectVectorVariable* const vectorVariable, const u32* data)
{
	vectorVariable->SetIntVector(reinterpret_cast<const s32*>(data));
}

template<>
inline void EffectVariable::SetVector<bool>(ID3DX11EffectVectorVariable* const vectorVariable, const bool* data)
{
	vectorVariable->SetBoolVector(data);
}

//-------------------------------------------------------------------------
// SetVectorArray<T>
//-------------------------------------------------------------------------

template<>
inline void EffectVariable::SetVectorArray<float>(
	ID3DX11EffectVectorVariable* const vectorVariable,
	const float* data, const std::size_t count
)
{
	vectorVariable->SetFloatVectorArray(data, 0, count);
}

template<>
inline void EffectVariable::SetVectorArray<s32>(
	ID3DX11EffectVectorVariable* const vectorVariable,
	const s32* data, const std::size_t count
)
{
	vectorVariable->SetIntVectorArray(data, 0, count);
}

template<>
inline void EffectVariable::SetVectorArray<u32>(
	ID3DX11EffectVectorVariable* const vectorVariable,
	const u32* data, const std::size_t count
)
{
	vectorVariable->SetIntVectorArray(reinterpret_cast<const s32*>(data), 0, count);
}

template<>
inline void EffectVariable::SetVectorArray<bool>(
	ID3DX11EffectVectorVariable* const vectorVariable,
	const bool* data, const std::size_t count
)
{
	vectorVariable->SetBoolVectorArray(data, 0, count);
}

//-------------------------------------------------------------------------
// operator <<
//-------------------------------------------------------------------------

template<typename T>
inline std::enable_if_t<!is_hlsl_scalar<T>, EffectVariable&>
EffectVariable::operator<<(const T& value)
{
	SetRaw(m_variable.Get(), value);
	return *this;
}

template<typename T>
inline std::enable_if_t<!is_hlsl_scalar<T>, EffectVariable&>
EffectVariable::operator<<(const std::vector<T>& value)
{
	SetRawArray(m_variable.Get(), value.data(), value.size());
	return *this;
}

template <typename T, std::size_t Count>
inline std::enable_if_t<!is_hlsl_scalar<T>, EffectVariable&>
EffectVariable::operator<<(const std::array<T, Count>& value)
{
	SetRawArray(m_variable.Get(), value.data(), Count);
	return *this;
}

//-------------------------------------------------------------------------

template<typename T>
inline std::enable_if_t<is_hlsl_scalar<T>, EffectVariable&>
EffectVariable::operator<<(const T value)
{
	if (auto scalarVariable = ToScalarVariable())
		SetScalar(scalarVariable, value);

	return *this;
}

template<typename T>
inline std::enable_if_t<is_hlsl_scalar<T>, EffectVariable&>
EffectVariable::operator<<(const std::vector<T>& value)
{
	if (auto scalarVariable = ToScalarVariable())
		SetScalarArray(scalarVariable, value.data(), value.size());

	return *this;
}

template <typename T, std::size_t Count>
inline std::enable_if_t<is_hlsl_scalar<T>, EffectVariable&>
EffectVariable::operator<<(const std::array<T, Count>& value)
{
	if (auto scalarVariable = ToScalarVariable())
		SetScalarArray(scalarVariable, value.data(), Count);

	return *this;
}

//-------------------------------------------------------------------------

template<typename T, unsigned Size>
inline EffectVariable& EffectVariable::operator<<(const math::VectorType<T, Size>& value)
{
	if (auto vectorVariable = ToVectorVariable())
		SetVector(vectorVariable, static_cast<const T*>(value));

	return *this;
}

template<typename T, unsigned Size>
inline EffectVariable& EffectVariable::operator<<(const math::VectorArray<T, Size>& value)
{
	if (auto vectorVariable = ToVectorVariable())
		SetVectorArray(vectorVariable, reinterpret_cast<const T*>(value.data()), value.size());

	return *this;
}

template <typename T, unsigned Size, std::size_t Count>
inline EffectVariable& EffectVariable::operator<<(const math::VectorFixedArray<T, Size, Count>& value)
{
	if (auto vectorVariable = ToVectorVariable())
		SetVectorArray(vectorVariable, reinterpret_cast<const T*>(value.data()), Count);

	return *this;
}

//-------------------------------------------------------------------------

template<unsigned Size>
inline EffectVariable& EffectVariable::operator<<(const math::Matrix<Size>& value)
{
	if (auto matrixVariable = ToMatrixVariable())
		SetMatrix(matrixVariable, static_cast<const float*>(value));

	return *this;
}

template<unsigned Size>
inline EffectVariable& EffectVariable::operator<<(const math::MatrixArray<Size>& value)
{
	if (auto matrixVariable = ToMatrixVariable())
		SetMatrixArray(matrixVariable, reinterpret_cast<const float*>(value.data()), value.size());

	return *this;
}

template<unsigned Size, unsigned Count>
inline EffectVariable& EffectVariable::operator<<(const math::MatrixFixedArray<Size, Count>& value)
{
	if (auto matrixVariable = ToMatrixVariable())
		SetMatrixArray(matrixVariable, reinterpret_cast<const float*>(value.data()), Count);

	return *this;
}
} // namespace library
