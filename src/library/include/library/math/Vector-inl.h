#pragma once

namespace library::math
{
// -----------------------------------------------------------------------------
// logical operators
// -----------------------------------------------------------------------------

template<typename T, unsigned Size>
bool operator==(const VectorType<T, Size>& lhs, const VectorType<T, Size>& rhs)
{
	return dx::StaticVector<T, Size>::Equal(dx::VECTOR(lhs), dx::VECTOR(rhs));
}

template<typename T, unsigned Size>
bool operator!=(const VectorType<T, Size>& lhs, const VectorType<T, Size>& rhs)
{
	return dx::StaticVector<T, Size>::NotEqual(dx::VECTOR(lhs), dx::VECTOR(rhs));
}

template<typename T, unsigned Size>
bool operator<(const VectorType<T, Size>& lhs, const VectorType<T, Size>& rhs)
{
	return dx::VectorLess<Size>(dx::VECTOR(lhs), dx::VECTOR(rhs));
}

template<typename T, unsigned Size>
bool operator<=(const VectorType<T, Size>& lhs, const VectorType<T, Size>& rhs)
{
	return dx::VectorLessOrEqual<Size>(dx::VECTOR(lhs), dx::VECTOR(rhs));
}

template<typename T, unsigned Size>
bool operator>(const VectorType<T, Size>& lhs, const VectorType<T, Size>& rhs)
{
	return dx::VectorGreater<Size>(dx::VECTOR(lhs), dx::VECTOR(rhs));
}

template<typename T, unsigned Size>
bool operator>=(const VectorType<T, Size>& lhs, const VectorType<T, Size>& rhs)
{
	return dx::VectorGreaterOrEqual<Size>(dx::VECTOR(lhs), dx::VECTOR(rhs));
}

//-------------------------------------------------------------------------
// arithmetic operators
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// add
//-------------------------------------------------------------------------

template<typename T, typename U, unsigned Size, typename C = std::common_type_t<T, U>>
VectorType<C, Size> operator + (const VectorType<T, Size>& lhs, const VectorType<U, Size>& rhs)
{
	return VectorType<C, Size>(dx::XMVectorAdd(dx::VECTOR(lhs), dx::VECTOR(rhs)));
}

template<typename T, typename U, unsigned Size>
VectorType<T, Size>& operator += (VectorType<T, Size>& lhs, const VectorType<U, Size>& rhs)
{
	return lhs = dx::XMVectorAdd(dx::VECTOR(lhs), dx::VECTOR(rhs));
}

template<typename T, typename U, unsigned Size, typename C = std::common_type_t<T, U>>
VectorType<C, Size> operator + (const VectorType<T, Size>& lhs, const U v)
{
	return VectorType<C, Size>(dx::XMVectorAdd(dx::VECTOR(lhs), dx::XMVectorReplicate(v)));
}

template<typename T, typename U, unsigned Size, typename C = std::common_type_t<T, U>>
VectorType<T, Size>& operator += (VectorType<T, Size>& lhs, const U v)
{
	return lhs = dx::XMVectorAdd(dx::VECTOR(lhs), dx::XMVectorReplicate(v));
}

//-------------------------------------------------------------------------
// negate
//-------------------------------------------------------------------------

template<typename T, unsigned Size>
VectorType<T, Size> operator - (const VectorType<T, Size>& vec)
{
	return VectorType<T, Size>(dx::XMVectorNegate(dx::VECTOR(vec)));
}

//-------------------------------------------------------------------------
// subtract
//-------------------------------------------------------------------------

template<typename T, typename U, unsigned Size, typename C = std::common_type_t<T, U>>
VectorType<C, Size> operator - (const VectorType<T, Size>& lhs, const VectorType<U, Size>& rhs)
{
	return VectorType<C, Size>(dx::XMVectorSubtract(dx::VECTOR(lhs), dx::VECTOR(rhs)));
}

template<typename T, typename U, unsigned Size>
VectorType<T, Size>& operator -= (VectorType<T, Size>& lhs, const VectorType<U, Size>& rhs)
{
	return lhs = dx::XMVectorSubtract(dx::VECTOR(lhs), dx::VECTOR(rhs));
}

template<typename T, typename U, unsigned Size, typename C = std::common_type_t<T, U>>
VectorType<C, Size> operator - (const VectorType<T, Size>& lhs, const U v)
{
	return VectorType<C, Size>(dx::XMVectorSubtract(dx::VECTOR(lhs), dx::XMVectorReplicate(v)));
}

template<typename T, typename U, unsigned Size, typename C = std::common_type_t<T, U>>
VectorType<T, Size>& operator -= (VectorType<T, Size>& lhs, const U v)
{
	return lhs = dx::XMVectorSubtract(dx::VECTOR(lhs), dx::XMVectorReplicate(v));
}

//-------------------------------------------------------------------------
// multiply
//-------------------------------------------------------------------------

template<typename T, typename U, unsigned Size, typename C = std::common_type_t<T, U>>
VectorType<C, Size> operator * (const VectorType<T, Size>& lhs, const VectorType<U, Size>& rhs)
{
	return VectorType<C, Size>(dx::XMVectorMultiply(dx::VECTOR(lhs), dx::VECTOR(rhs)));
}

template<typename T, typename U, unsigned Size>
VectorType<T, Size>& operator *= (VectorType<T, Size>& lhs, const VectorType<U, Size>& rhs)
{
	return lhs = dx::XMVectorMultiply(dx::VECTOR(lhs), dx::VECTOR(rhs));
}

template<typename T, typename U, unsigned Size, typename C = std::common_type_t<T, U>>
VectorType<C, Size> operator * (const VectorType<T, Size>& lhs, const U v)
{
	return VectorType<C, Size>(dx::XMVectorMultiply(dx::VECTOR(lhs), dx::XMVectorReplicate(v)));
}

template<typename T, typename U, unsigned Size, typename C = std::common_type_t<T, U>>
VectorType<T, Size>& operator *= (VectorType<T, Size>& lhs, const U v)
{
	return lhs = dx::XMVectorMultiply(dx::VECTOR(lhs), dx::XMVectorReplicate(v));
}

//-------------------------------------------------------------------------
// divide
//-------------------------------------------------------------------------

template<typename T, typename U, unsigned Size, typename C = std::common_type_t<T, U>>
VectorType<C, Size> operator / (const VectorType<T, Size>& lhs, const VectorType<U, Size>& rhs)
{
	return VectorType<C, Size>(dx::XMVectorDivide(dx::VECTOR(lhs), dx::VECTOR(rhs)));
}

template<typename T, typename U, unsigned Size>
VectorType<T, Size>& operator /= (VectorType<T, Size>& lhs, const VectorType<U, Size>& rhs)
{
	return lhs = dx::XMVectorDivide(dx::VECTOR(lhs), dx::VECTOR(rhs));
}

template<typename T, typename U, unsigned Size, typename C = std::common_type_t<T, U>>
VectorType<C, Size> operator / (const VectorType<T, Size>& lhs, const U v)
{
	return VectorType<C, Size>(dx::XMVectorDivide(dx::VECTOR(lhs), dx::XMVectorReplicate(v)));
}

template<typename T, typename U, unsigned Size, typename C = std::common_type_t<T, U>>
VectorType<T, Size>& operator /= (VectorType<T, Size>& lhs, const U v)
{
	return lhs = dx::XMVectorDivide(dx::VECTOR(lhs), dx::XMVectorReplicate(v));
}
} // namespace library::math
