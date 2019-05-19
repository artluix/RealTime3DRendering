#include "StdAfx.h"
#include "library/Math/Quaternion.h"

#include "library/Math/Matrix.h"

namespace library::math
{
Quaternion& Quaternion::operator=(const XMVector& xmVector)
{
	DirectX::XMStoreFloat4(reinterpret_cast<XMVector4*>(this), xmVector);
	return *this;
}

//-------------------------------------------------------------------------

Quaternion::operator XMVector() const
{
	return DirectX::XMLoadFloat4(reinterpret_cast<const XMVector4*>(this));
}

Quaternion::operator const XMVector4&() const
{
	return reinterpret_cast<const XMVector4&>(*this);
}

Quaternion::operator bool() const
{
	static constexpr Quaternion zero;
	return *this != zero;
}

//-------------------------------------------------------------------------

const Quaternion Quaternion::Identity{0.f, 0.f, 0.f, 1.f};

//-------------------------------------------------------------------------

Quaternion Quaternion::RotationAxis(const Vector3& axis, const float angle)
{
	return Quaternion(DirectX::XMQuaternionRotationAxis(XMVector(axis), angle));
}

Quaternion Quaternion::RotationMatrix(const Matrix4& matrix)
{
	return Quaternion(DirectX::XMQuaternionRotationMatrix(XMMatrix(matrix)));
}

Quaternion Quaternion::RotationPitchYawRoll(const float pitch, const float yaw, const float roll)
{
	return Quaternion(DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
}

Quaternion Quaternion::RotationPitchYawRoll(const Vector3& vector)
{
	return Quaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(XMVector(vector)));
}

Quaternion Quaternion::SLerp(const Quaternion& lhs, const Quaternion& rhs, const float factor)
{
	return Quaternion(DirectX::XMQuaternionSlerp(XMVector(lhs), XMVector(rhs), factor));
}

//-------------------------------------------------------------------------

float Quaternion::Length() const
{
	return DirectX::XMVectorGetX(DirectX::XMQuaternionLength(XMVector(*this)));
}

float Quaternion::LengthSq() const
{
	return DirectX::XMVectorGetX(DirectX::XMQuaternionLengthSq(XMVector(*this)));
}

float Quaternion::Dot(const Quaternion& other) const
{
	return DirectX::XMVectorGetX(DirectX::XMQuaternionDot(XMVector(*this), XMVector(other)));
}

Quaternion Quaternion::Normalize() const
{
	return Quaternion(DirectX::XMQuaternionNormalize(XMVector(*this)));
}

Quaternion Quaternion::Conjugate() const
{
	return Quaternion(DirectX::XMQuaternionConjugate(XMVector(*this)));
}

Quaternion Quaternion::Inverse() const
{
	return Quaternion(DirectX::XMQuaternionInverse(XMVector(*this)));
}

//-------------------------------------------------------------------------

bool Quaternion::operator==(const Quaternion& other) const
{
	return DirectX::XMQuaternionEqual(XMVector(*this), XMVector(other));
}

bool Quaternion::operator!=(const Quaternion& other) const
{
	return DirectX::XMQuaternionEqual(XMVector(*this), XMVector(other));
}

//-------------------------------------------------------------------------
// multiply
//-------------------------------------------------------------------------

Quaternion Quaternion::operator*(const Quaternion& other) const
{
	return Quaternion(DirectX::XMQuaternionMultiply(XMVector(*this), XMVector(other)));
}

Quaternion& Quaternion::operator*=(const Quaternion& other)
{
	return *this = DirectX::XMVectorMultiply(XMVector(*this), XMVector(other));
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
} // namespace library::math
