#include "StdAfx.h"
#include "library/Math/Quaternion.h"

#include "library/Math/Matrix.h"

namespace library::math
{
//-------------------------------------------------------------------------

const Quaternion Quaternion::Zero{ 0.f };
const Quaternion Quaternion::Identity{ 0.f, 0.f, 0.f, 1.f };

//-------------------------------------------------------------------------

Quaternion Quaternion::RotationBetweenVectors(const Vector3& from, const Vector3& to)
{
	const float normFromTo = std::sqrt(from.LengthSq() * to.LengthSq());
	const float dot = from.Dot(to);
	float realPart = normFromTo + dot;

	Vector3 w;

	if (dot < 1.e-6f)
	{
		/* If from and to are exactly opposite, rotate 180 degrees
		 * around an arbitrary orthogonal axis. Axis normalization
		 * can happen later, when we normalize the quaternion. */
		realPart = 0.f;
		w = (abs(from.x) > abs(from.z)) ?
			Vector3(-from.y, from.x, 0.f) :
			Vector3(0.f, -from.z, from.y);
	}
	else
	{
		/* Otherwise, build quaternion the standard way. */
		w = from.Cross(to);
	}

	return Quaternion(w.x, w.y, w.z, realPart).Normalize();
}

Quaternion Quaternion::RotationAxis(const Vector3& axis, const float angle)
{
	return Quaternion(dx::XMQuaternionRotationAxis(dx::VECTOR(axis), angle));
}

Quaternion Quaternion::RotationMatrix(const Matrix4& matrix)
{
	return Quaternion(dx::XMQuaternionRotationMatrix(dx::MATRIX(matrix)));
}

Quaternion Quaternion::RotationPitchYawRoll(const float pitch, const float yaw, const float roll)
{
	return Quaternion(dx::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
}

Quaternion Quaternion::RotationPitchYawRoll(const Vector3& vector)
{
	return Quaternion(dx::XMQuaternionRotationRollPitchYawFromVector(dx::VECTOR(vector)));
}

Quaternion Quaternion::SLerp(const Quaternion& lhs, const Quaternion& rhs, const float factor)
{
	return Quaternion(dx::XMQuaternionSlerp(dx::VECTOR(lhs), dx::VECTOR(rhs), factor));
}

//-------------------------------------------------------------------------

Quaternion Quaternion::Normalize() const
{
	return Quaternion(dx::XMQuaternionNormalize(dx::VECTOR(*this)));
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion(dx::XMQuaternionConjugate(dx::VECTOR(*this)));
}

Quaternion Quaternion::Inverse() const
{
	return Quaternion(dx::XMQuaternionInverse(dx::VECTOR(*this)));
}

//-------------------------------------------------------------------------

bool Quaternion::operator==(const Quaternion& other) const
{
	return static_cast<const Vector4&>(*this) == static_cast<const Vector4&>(other);
}

bool Quaternion::operator!=(const Quaternion& other) const
{
	return static_cast<const Vector4&>(*this) != static_cast<const Vector4&>(other);
}

//-------------------------------------------------------------------------
// multiply
//-------------------------------------------------------------------------

Quaternion Quaternion::operator*(const Quaternion& other) const
{
	return Quaternion(dx::XMQuaternionMultiply(dx::VECTOR(*this), dx::VECTOR(other)));
}

Quaternion& Quaternion::operator*=(const Quaternion& other)
{
	return *this = dx::XMQuaternionMultiply(dx::VECTOR(*this), dx::VECTOR(other));
}

//-------------------------------------------------------------------------

Quaternion Quaternion::operator*(const float value) const
{
	return *this * Quaternion(value);
}

Quaternion& Quaternion::operator*=(const float value)
{
	return *this *= Quaternion(value);
}

//-------------------------------------------------------------------------
// Clamp & Lerp
//-------------------------------------------------------------------------

Quaternion Clamp(const Quaternion& q, const Quaternion& min, const Quaternion& max)
{
	return Quaternion(Clamp(
		static_cast<const Vector4&>(q),
		static_cast<const Vector4&>(min),
		static_cast<const Vector4&>(max)
	));
}

Quaternion Lerp(const Quaternion& from, const Quaternion& to, const float factor)
{
	return Quaternion(Lerp(static_cast<const Vector4&>(from), static_cast<const Vector4&>(to), factor));
}

Quaternion Lerp(const Quaternion& from, const Quaternion& to, const Vector4& factor)
{
	return Quaternion(Lerp(static_cast<const Vector4&>(from), static_cast<const Vector4&>(to), factor));
}
} // namespace library::math
