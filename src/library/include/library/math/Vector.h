#pragma once
#include "library/Math/DxMath.h"

#include <string>

namespace library::math
{
//-------------------------------------------------------------------------
// VectorType<T, 2>
//-------------------------------------------------------------------------

template<typename T>
struct VectorType<T, 2>
{
	union
	{
		struct
		{
			T x, y;
		};

		dx::Vector2<T> _dxVec;
		T _data[2];
	};

	constexpr VectorType(const T x, const T y) : x(x), y(y) {}
	explicit constexpr VectorType(const T v = T(0)) : VectorType(v, v) {}

	template<typename U>
	explicit constexpr VectorType(const VectorType2<U>& other);

	explicit VectorType(const dx::VECTOR& dxVEC);
	VectorType& operator=(const dx::VECTOR& dxVEC);
	explicit operator dx::VECTOR() const;

	//-------------------------------------------------------------------------

	static const VectorType Zero;
	static const VectorType One;
	
	//-------------------------------------------------------------------------

	explicit operator bool() const;

	const T& operator [](const unsigned idx) const { return _data[idx]; }
	T& operator [](const unsigned idx) { return _data[idx]; }
	explicit operator const T*() const { return _data; }

	//-------------------------------------------------------------------------

	VectorType2<float> Normalize() const;

	template<typename U = T, typename = enable_if_float_t<U, T>>
	float Length() const;

	template<typename U, typename C = std::common_type_t<U, T>>
	C Dot(const VectorType2<U>& other) const;

	template<typename U, typename C = std::common_type_t<U, T>>
	VectorType2<C> Cross(const VectorType2<U>& other) const;

	T LengthSq() const { return Dot(*this); }

	VectorType2<float> Transform(const Matrix4& mat) const;

	//-------------------------------------------------------------------------

	std::string ToString() const;
};

//-------------------------------------------------------------------------
// VectorType<T, 3>
//-------------------------------------------------------------------------

template<typename T>
struct VectorType<T, 3>
{
	union
	{
		struct
		{
			T x, y, z;
		};

		VectorType2<T> xy;

		dx::Vector3<T> _dxVec;
		T _data[3];
	};

	constexpr VectorType(const T x, const T y, const T z) : x(x), y(y), z(z) {}
	explicit constexpr VectorType(const T v = T(0)) : VectorType(v, v, v) {}
	explicit constexpr VectorType(const VectorType<T, 2>& xy, const T z = T(0));

	template<typename U>
	explicit constexpr VectorType(const VectorType3<U>& other);

	explicit VectorType(const dx::VECTOR& dxVEC);
	VectorType& operator=(const dx::VECTOR& dxVEC);
	explicit operator dx::VECTOR() const;

	//-------------------------------------------------------------------------

	static const VectorType Zero;
	static const VectorType One;

	//-------------------------------------------------------------------------

	explicit operator bool() const;

	const T& operator [](const unsigned idx) const { return _data[idx]; }
	T& operator [](const unsigned idx) { return _data[idx]; }
	explicit operator const T*() const { return _data; }

	//-------------------------------------------------------------------------

	VectorType3<float> Normalize() const;

	template<typename U = T, typename = enable_if_float_t<U, T>>
	float Length() const;

	template<typename U, typename C = std::common_type_t<U, T>>
	C Dot(const VectorType3<U>& other) const;

	template<typename U, typename C = std::common_type_t<U, T>>
	VectorType3<C> Cross(const VectorType3<U>& other) const;

	T LengthSq() const { return Dot(*this); }

	VectorType3<float> Transform(const Matrix4& mat) const;

	std::string ToString() const;
};

//-------------------------------------------------------------------------
// VectorType<T, 4>
//-------------------------------------------------------------------------

template<typename T>
struct VectorType<T, 4>
{
	union
	{
		struct
		{
			T x, y, z, w;
		};

		VectorType3<T> xyz;

		dx::Vector4<T> _dxVec;
		T _data[4];
	};

	constexpr VectorType(const T x, const T y, const T z, const T w) : x(x), y(y), z(z), w(w) {}
	explicit constexpr VectorType(const T v = T(0)) : VectorType(v, v, v, v) {}
	explicit constexpr VectorType(const VectorType<T, 3>& xyz, const T w = T(0));

	template<typename U>
	explicit constexpr VectorType(const VectorType4<U>& other);

	explicit VectorType(const dx::VECTOR& dxVEC);
	VectorType& operator=(const dx::VECTOR& dxVEC);
	explicit operator dx::VECTOR() const;

	//-------------------------------------------------------------------------

	static const VectorType Zero;
	static const VectorType One;

	//-------------------------------------------------------------------------

	explicit operator bool() const;

	const T& operator [](const unsigned idx) const { return _data[idx]; }
	T& operator [](const unsigned idx) { return _data[idx]; }
	explicit operator const T*() const { return _data; }

	//-------------------------------------------------------------------------

	VectorType4<float> Normalize() const;

	template<typename U = T, typename = enable_if_float_t<U, T>>
	float Length() const;

	template<typename U, typename C = std::common_type_t<U, T>>
	C Dot(const VectorType4<U>& other) const;

	T LengthSq() const { return Dot(*this); }

	VectorType4<float> Transform(const Matrix4& mat) const;

	//-------------------------------------------------------------------------

	std::string ToString() const;
};

//-------------------------------------------------------------------------
// extern template instantiation
//-------------------------------------------------------------------------

extern template struct VectorType<float, 2>;
extern template struct VectorType<s32, 2>;
extern template struct VectorType<u32, 2>;

extern template struct VectorType<float, 3>;
extern template struct VectorType<s32, 3>;
extern template struct VectorType<u32, 3>;

extern template struct VectorType<float, 4>;
extern template struct VectorType<s32, 4>;
extern template struct VectorType<u32, 4>;

// -----------------------------------------------------------------------------
// Clamp and Lerp
// -----------------------------------------------------------------------------

template<typename T, unsigned Size>
inline VectorType<T, Size> Clamp(
	const VectorType<T, Size>& v,
	const VectorType<T, Size>& min,
	const VectorType<T, Size>& max
)
{
	return VectorType<T, Size>(dx::XMVectorClamp(dx::VECTOR(v), dx::VECTOR(min), dx::VECTOR(max)));
}

template<typename T, unsigned Size>
inline VectorType<T, Size> Lerp(
	const VectorType<T, Size>& from,
	const VectorType<T, Size>& to,
	const float factor)
{
	return VectorType<T, Size>(dx::XMVectorLerp(dx::VECTOR(from), dx::VECTOR(to), factor));
}

template<typename T, unsigned Size>
inline VectorType<T, Size> Lerp(
	const VectorType<T, Size>& from,
	const VectorType<T, Size>& to,
	const VectorType<float, Size>& factor)
{
	return VectorType<T, Size>(dx::XMVectorLerpV(dx::VECTOR(from), dx::VECTOR(to), dx::VECTOR(factor)));
}
} // namespace library::math

#include "library/Math/Vector-inl.h"