#pragma once
#include "library/Math/Vector.h"

namespace library::math
{
struct Quaternion
{
	union
	{
		struct
		{
			float x, y, z, w;
		};

		std::array<float, 4> _data;
	};

	//-------------------------------------------------------------------------

	constexpr Quaternion(const float x, const float y, const float z, const float w) : x(x), y(y), z(z), w(w)
	{}
	explicit constexpr Quaternion(const float v = 0.f) : Quaternion(v, v, v, v) {}
	explicit constexpr Quaternion(const XMVector4& xmFloat4)
		: Quaternion(xmFloat4.x, xmFloat4.y, xmFloat4.z, xmFloat4.w)
	{}
	explicit constexpr Quaternion(const XMVector& xmVector)
		: Quaternion(xmVector.m128_f32[0], xmVector.m128_f32[1], xmVector.m128_f32[2], xmVector.m128_f32[3])
	{}

	Quaternion& operator=(const XMVector& xmVector);

	//-------------------------------------------------------------------------

	const float& operator[](const unsigned idx) const { return _data[idx]; }
	float& operator[](const unsigned idx) { return _data[idx]; }
	operator const float*() const { return _data.data(); }

	explicit operator XMVector() const;
	explicit operator const XMVector4&() const;
	explicit operator bool() const;

	//-------------------------------------------------------------------------

	static const Quaternion Identity;

	//-------------------------------------------------------------------------

	static Quaternion RotationAxis(const Vector3& axis, const float angle);
	static Quaternion RotationMatrix(const Matrix4& matrix);
	static Quaternion RotationPitchYawRoll(const float pitch, const float yaw, const float roll);
	static Quaternion RotationPitchYawRoll(const Vector3& vector);

	static Quaternion SLerp(const Quaternion& lhs, const Quaternion& rhs, const float factor);

	//-------------------------------------------------------------------------

	float Length() const;
	float LengthSq() const;
	float Dot(const Quaternion& other) const;

	Quaternion Normalize() const;
	Quaternion Conjugate() const;
	Quaternion Inverse() const;

	//-------------------------------------------------------------------------

	bool operator==(const Quaternion& other) const;
	bool operator!=(const Quaternion& other) const;

	bool operator<(const Quaternion&) const = delete;
	bool operator<=(const Quaternion&) const = delete;
	bool operator>(const Quaternion&) const = delete;
	bool operator>=(const Quaternion&) const = delete;

	//-------------------------------------------------------------------------

	Quaternion operator*(const Quaternion& other) const;
	Quaternion& operator*=(const Quaternion& other);

	Quaternion operator*(const float value) const;
	Quaternion& operator*=(const float value);
};
} // namespace library::math
