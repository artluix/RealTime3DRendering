#pragma once
#include "library/Math/Vector.h"

namespace library::math
{
struct Quaternion : Vector4
{
	using Vector4::xyz;

public:
	using Vector4::x;
	using Vector4::y;
	using Vector4::z;
	using Vector4::w;

	using Vector4::Vector4;
	using Vector4::operator=;

	constexpr Quaternion(const Vector4& vec4) : Quaternion(vec4.x, vec4.y, vec4.z, vec4.w) {}
	Quaternion& operator=(const Vector4& vec4) { Vector4::operator=(vec4); return *this; }

	Quaternion& operator=(const dx::VECTOR& VEC) { Vector4::operator=(VEC); return *this; }

	//-------------------------------------------------------------------------

	static const Quaternion Zero;
	static const Quaternion Identity;

	//-------------------------------------------------------------------------

	static Quaternion RotationAxis(const Vector3& axis, const float angle);
	static Quaternion RotationMatrix(const Matrix4& matrix);
	static Quaternion RotationPitchYawRoll(const float pitch, const float yaw, const float roll);
	static Quaternion RotationPitchYawRoll(const Vector3& vector);

	static Quaternion SLerp(const Quaternion& lhs, const Quaternion& rhs, const float factor);

	//-------------------------------------------------------------------------

	float Dot(const Quaternion& other) const { return Vector4::Dot(other); }

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

//-------------------------------------------------------------------------

Quaternion Clamp(const Quaternion& q, const Quaternion& min, const Quaternion& max);
Quaternion Lerp(const Quaternion& from, const Quaternion& to, const float factor);
Quaternion Lerp(const Quaternion& from, const Quaternion& to, const Vector4& factor);
} // namespace library::math
