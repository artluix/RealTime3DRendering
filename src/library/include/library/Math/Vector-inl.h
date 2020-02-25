#pragma once

namespace library::math
{
//-------------------------------------------------------------------------
// VectorType2<T>
//-------------------------------------------------------------------------

template<typename T>
template<typename U>
constexpr VectorType<T, 2>::VectorType(const VectorType2<U>& other)
	: VectorType(
		static_cast<T>(other.x),
		static_cast<T>(other.y)
	)
{}

template<typename T>
template<typename U, typename>
float VectorType<T, 2>::Length() const
{
	return dx::VectorLength<2>(dx::VECTOR(*this));
}

template<typename T>
template<typename U, typename C>
C VectorType<T, 2>::Dot(const VectorType2<U>& other) const
{
	return static_cast<C>(dx::VectorDot<2>(dx::VECTOR(*this), dx::VECTOR(other)));
}

template<typename T>
template<typename U, typename C>
VectorType2<C> VectorType<T, 2>::Cross(const VectorType2<U>& other) const
{
	return VectorType2<C>(dx::VectorCross<2>(dx::VECTOR(*this), dx::VECTOR(other)));
}

//-------------------------------------------------------------------------
// VectorType3<T>
//-------------------------------------------------------------------------

template<typename T>
template<typename U>
constexpr VectorType<T, 3>::VectorType(const VectorType3<U>& other)
	: VectorType(
		static_cast<T>(other.x),
		static_cast<T>(other.y),
		static_cast<T>(other.z)
	)
{}

template<typename T>
template<typename U, typename>
float VectorType<T, 3>::Length() const
{
	return dx::VectorLength<3>(dx::VECTOR(*this));
}

template<typename T>
template<typename U, typename C>
C VectorType<T, 3>::Dot(const VectorType3<U>& other) const
{
	return static_cast<C>(dx::VectorDot<3>(dx::VECTOR(*this), dx::VECTOR(other)));
}

template<typename T>
template<typename U, typename C>
VectorType3<C> VectorType<T, 3>::Cross(const VectorType3<U>& other) const
{
	return VectorType3<C>(dx::VectorCross<3>(dx::VECTOR(*this), dx::VECTOR(other)));
}

//-------------------------------------------------------------------------
// VectorType4<T>
//-------------------------------------------------------------------------

template<typename T>
template<typename U>
constexpr VectorType<T, 4>::VectorType(const VectorType4<U>& other)
	: VectorType(
		static_cast<T>(other.x),
		static_cast<T>(other.y),
		static_cast<T>(other.z),
		static_cast<T>(other.w)
	)
{}

template<typename T>
template<typename U, typename>
float VectorType<T, 4>::Length() const
{
	return dx::VectorLength<4>(dx::VECTOR(*this));
}

template<typename T>
template<typename U, typename C>
C VectorType<T, 4>::Dot(const VectorType4<U>& other) const
{
	return static_cast<C>(dx::VectorDot<4>(dx::VECTOR(*this), dx::VECTOR(other)));
}

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
