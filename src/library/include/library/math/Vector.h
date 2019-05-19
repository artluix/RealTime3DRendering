#pragma once
#include "library/Math/XmTypes.h"

#include <array>
#include <vector>
#include <string>

namespace library::math
{
template <unsigned Size>
struct Vector;

using Vector2 = Vector<2>;
using Vector3 = Vector<3>;
using Vector4 = Vector<4>;

template <class T>
constexpr bool IsVector2 = std::is_base_of_v<Vector2, T>;

template <class T>
constexpr bool IsVector3 = std::is_base_of_v<Vector3, T>;

template <class T>
constexpr bool IsVector4 = std::is_base_of_v<Vector4, T>;

template <class T>
constexpr bool IsVector = IsVector2<T> || IsVector3<T> || IsVector4<T>;

//-------------------------------------------------------------------------

template <unsigned Size>
struct Matrix;

// using Matrix2 = Matrix<2>;
using Matrix3 = Matrix<3>;
using Matrix4 = Matrix<4>;

template <class T>
constexpr bool IsMatrix3 = std::is_base_of_v<Matrix3, T>;

template <class T>
constexpr bool IsMatrix4 = std::is_base_of_v<Matrix3, T>;

template <class T>
constexpr bool IsMatrix = IsMatrix3<T> || IsMatrix4<T>;

//-------------------------------------------------------------------------
// VectorDef<T>
//-------------------------------------------------------------------------

template <unsigned Size>
struct VectorDef
{
	static constexpr unsigned Size = Size;
};

//-------------------------------------------------------------------------
// Vector2
//-------------------------------------------------------------------------

template <>
struct Vector<2> : public VectorDef<2>
{
	union
	{
		struct
		{
			float x, y;
		};

		std::array<float, 2> _data;
	};

	//-------------------------------------------------------------------------

	constexpr Vector(const float x, const float y) : x(x), y(y) {}
	explicit constexpr Vector(const float v = 0.f) : Vector(v, v) {}
	explicit constexpr Vector(const XMVector2& xmFloat2) : Vector(xmFloat2.x, xmFloat2.y) {}
	explicit constexpr Vector(const XMVector& xmVector) : Vector(xmVector.m128_f32[0], xmVector.m128_f32[1])
	{}

	Vector& operator=(const XMVector& xmVector);

	//-------------------------------------------------------------------------

	const float& operator[](const unsigned idx) const { return _data[idx]; }
	float& operator[](const unsigned idx) { return _data[idx]; }
	operator const float*() const { return _data.data(); }

	//-------------------------------------------------------------------------

	explicit operator XMVector() const;
	explicit operator const XMVector2&() const;
	explicit operator bool() const;

	//-------------------------------------------------------------------------

	static const Vector Zero;
	static const Vector One;

	//-------------------------------------------------------------------------

	std::string ToString() const;

	//-------------------------------------------------------------------------

	float Length() const;
	float LengthSq() const;
	float Dot(const Vector& other) const;

	Vector Normalize() const;
	Vector Transform(const Matrix3& matrix) const;

	//-------------------------------------------------------------------------
	// logical operators
	//-------------------------------------------------------------------------

	bool operator==(const Vector& other) const;
	bool operator!=(const Vector& other) const;
	bool operator<(const Vector& other) const;
	bool operator<=(const Vector& other) const;
	bool operator>(const Vector& other) const;
	bool operator>=(const Vector& other) const;
};

//-------------------------------------------------------------------------
// Vector3
//-------------------------------------------------------------------------

template <>
struct Vector<3> : public VectorDef<3>
{
	union
	{
		struct
		{
			float x, y, z;
		};

		std::array<float, 3> _data;
	};

	//-------------------------------------------------------------------------

	constexpr Vector(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	explicit constexpr Vector(const float v = 0.f) : Vector(v, v, v) {}
	explicit constexpr Vector(const XMVector3& xmFloat3) : Vector(xmFloat3.x, xmFloat3.y, xmFloat3.z) {}
	explicit constexpr Vector(const XMVector& xmVector)
		: Vector(xmVector.m128_f32[0], xmVector.m128_f32[1], xmVector.m128_f32[2])
	{}

	Vector& operator=(const XMVector& xmVector);

	//-------------------------------------------------------------------------

	const float& operator[](const unsigned idx) const { return _data[idx]; }
	float& operator[](const unsigned idx) { return _data[idx]; }
	operator const float*() const { return _data.data(); }

	//-------------------------------------------------------------------------

	explicit operator XMVector() const;
	explicit operator const XMVector3&() const;
	explicit operator bool() const;

	//-------------------------------------------------------------------------

	static const Vector Zero;
	static const Vector One;

	static const Vector Forward;
	static const Vector Backward;
	static const Vector Up;
	static const Vector Down;
	static const Vector Right;
	static const Vector Left;

	//-------------------------------------------------------------------------

	std::string ToString() const;

	//-------------------------------------------------------------------------

	float Length() const;
	float LengthSq() const;

	float Dot(const Vector& other) const;
	Vector Cross(const Vector& other) const;

	Vector Normalize() const;
	Vector Transform(const Matrix4& matrix) const;
	Vector TransformNormal(const Matrix3& matrix) const;

	//-------------------------------------------------------------------------
	// logical operators
	//-------------------------------------------------------------------------

	bool operator==(const Vector& other) const;
	bool operator!=(const Vector& other) const;
	bool operator<(const Vector& other) const;
	bool operator<=(const Vector& other) const;
	bool operator>(const Vector& other) const;
	bool operator>=(const Vector& other) const;
};

//-------------------------------------------------------------------------
// Vector4
//-------------------------------------------------------------------------

template <>
struct Vector<4> : public VectorDef<4>
{
	union
	{
		// Vector components
		struct
		{
			float x, y, z, w;
		};

		// Vector3 & w
		struct
		{
			Vector3 xyz;
			float _w;
		};

		std::array<float, 4> _data;
	};

	//-------------------------------------------------------------------------

	constexpr Vector(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w) {}
	explicit constexpr Vector(const float v = 0.f) : Vector(v, v, v, v) {}
	explicit constexpr Vector(const XMVector4& xmFloat4)
		: Vector(xmFloat4.x, xmFloat4.y, xmFloat4.z, xmFloat4.w)
	{}
	explicit constexpr Vector(const XMVector& xmVector)
		: Vector(xmVector.m128_f32[0], xmVector.m128_f32[1], xmVector.m128_f32[2], xmVector.m128_f32[3])
	{}

	Vector& operator=(const XMVector& xmVector);

	//-------------------------------------------------------------------------

	const float& operator[](const unsigned idx) const { return _data[idx]; }
	float& operator[](const unsigned idx) { return _data[idx]; }
	operator const float*() const { return _data.data(); }

	//-------------------------------------------------------------------------

	explicit operator XMVector() const;
	explicit operator const XMVector4&() const;
	explicit operator bool() const;

	//-------------------------------------------------------------------------

	static const Vector Zero;
	static const Vector One;

	//-------------------------------------------------------------------------

	std::string ToString() const;

	//------------------------------------------------------------------------

	float Length() const;
	float LengthSq() const;
	float Dot(const Vector& other) const;

	Vector Normalize() const;
	Vector Transform(const Matrix4& matrix) const;

	//-------------------------------------------------------------------------
	// logical operators
	//-------------------------------------------------------------------------

	bool operator==(const Vector& other) const;
	bool operator!=(const Vector& other) const;
	bool operator<(const Vector& other) const;
	bool operator<=(const Vector& other) const;
	bool operator>(const Vector& other) const;
	bool operator>=(const Vector& other) const;
};

//-------------------------------------------------------------------------
// explicit template instantiation
//-------------------------------------------------------------------------

template struct Vector<2>;
template struct Vector<3>;
template struct Vector<4>;

//-------------------------------------------------------------------------
// arithmetic operators
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// add
//-------------------------------------------------------------------------

template <unsigned Size>
inline Vector<Size> operator+(const Vector<Size>& lhs, const Vector<Size>& rhs)
{
	return Vector<Size>(DirectX::XMVectorAdd(XMVector(lhs), XMVector(rhs)));
}

template <unsigned Size>
inline Vector<Size>& operator+=(Vector<Size>& lhs, const Vector<Size>& rhs)
{
	return lhs = Vector<Size>(DirectX::XMVectorAdd(XMVector(lhs), XMVector(rhs)));
}

//-------------------------------------------------------------------------

template <unsigned Size>
inline Vector<Size> operator+(const Vector<Size>& lhs, const float value)
{
	return lhs + Vector<Size>(value);
}

template <unsigned Size>
inline Vector<Size>& operator+=(Vector<Size>& lhs, const float value)
{
	return lhs += Vector<Size>(value);
}

//-------------------------------------------------------------------------
// negate
//-------------------------------------------------------------------------

template <unsigned Size>
inline Vector<Size> operator-(const Vector<Size>& vector)
{
	return Vector<Size>(DirectX::XMVectorNegate(XMVector(vector)));
}

//-------------------------------------------------------------------------
// subtract
//-------------------------------------------------------------------------

template <unsigned Size>
inline Vector<Size> operator-(const Vector<Size>& lhs, const Vector<Size>& rhs)
{
	return Vector<Size>(DirectX::XMVectorSubtract(XMVector(lhs), XMVector(rhs)));
}

template <unsigned Size>
inline Vector<Size>& operator-=(Vector<Size>& lhs, const Vector<Size>& rhs)
{
	return lhs = Vector<Size>(DirectX::XMVectorSubtract(XMVector(lhs), XMVector(rhs)));
}

//-------------------------------------------------------------------------

template <unsigned Size>
inline Vector<Size> operator-(const Vector<Size>& lhs, const float value)
{
	return lhs - Vector<Size>(value);
}

template <unsigned Size>
inline Vector<Size>& operator-=(Vector<Size>& lhs, const float value)
{
	return lhs -= Vector<Size>(value);
}

//-------------------------------------------------------------------------
// multiply
//-------------------------------------------------------------------------

template <unsigned Size>
inline Vector<Size> operator*(const Vector<Size>& lhs, const Vector<Size>& rhs)
{
	return Vector<Size>(DirectX::XMVectorMultiply(XMVector(lhs), XMVector(rhs)));
}

template <unsigned Size>
inline Vector<Size>& operator*=(Vector<Size>& lhs, const Vector<Size>& rhs)
{
	return lhs = Vector<Size>(DirectX::XMVectorMultiply(XMVector(lhs), XMVector(rhs)));
}

//-------------------------------------------------------------------------

template <unsigned Size>
inline Vector<Size> operator*(const Vector<Size>& lhs, const float value)
{
	return lhs * Vector<Size>(value);
}

template <unsigned Size>
inline Vector<Size>& operator*=(Vector<Size>& lhs, const float value)
{
	return lhs *= Vector<Size>(value);
}

//-------------------------------------------------------------------------
// divide
//-------------------------------------------------------------------------

template <unsigned Size>
inline Vector<Size> operator/(const Vector<Size>& lhs, const Vector<Size>& rhs)
{
	return Vector<Size>(DirectX::XMVectorDivide(XMVector(lhs), XMVector(rhs)));
}

template <unsigned Size>
inline Vector<Size> operator/=(Vector<Size>& lhs, const Vector<Size>& rhs)
{
	return lhs = Vector<Size>(DirectX::XMVectorDivide(XMVector(lhs), XMVector(rhs)));
}

//-------------------------------------------------------------------------

template <unsigned Size>
inline Vector<Size> operator/(const Vector<Size>& lhs, const float value)
{
	return lhs / Vector<Size>(value);
}

template <unsigned Size>
inline Vector<Size>& operator/=(Vector<Size>& lhs, const float value)
{
	return lhs /= Vector<Size>(value);
}

//-------------------------------------------------------------------------
// Lerp
//-------------------------------------------------------------------------

template <unsigned Size>
Vector<Size> Lerp(const Vector<Size>& lhs, const Vector<Size>& rhs, const float factor)
{
	return Vector<Size>(DirectX::XMVectorLerp(XMVector(lhs), XMVector(rhs), factor));
}

template <unsigned Size>
Vector<Size> Lerp(const Vector<Size>& lhs, const Vector<Size>& rhs, const Vector<Size> factor)
{
	return Vector<Size>(DirectX::XMVectorLerpV(XMVector(lhs), XMVector(rhs), XMVector(factor)));
}
} // namespace library::math
