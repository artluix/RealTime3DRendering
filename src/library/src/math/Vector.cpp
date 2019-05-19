#include "StdAfx.h"
#include "library/Math/Vector.h"

#include "library/Math/Matrix.h"

#include <sstream>
#include <iomanip>

namespace library::math
{
//-------------------------------------------------------------------------
// Vector2
//-------------------------------------------------------------------------

Vector<2>& Vector<2>::operator=(const XMVector& xmVector)
{
	DirectX::XMStoreFloat2(reinterpret_cast<XMVector2*>(this), xmVector);
	return *this;
}

//-------------------------------------------------------------------------

Vector<2>::operator XMVector() const
{
	return DirectX::XMLoadFloat2(reinterpret_cast<const XMVector2*>(this));
}

Vector<2>::operator const XMVector2&() const
{
	return reinterpret_cast<const XMVector2&>(*this);
}

Vector<2>::operator bool() const
{
	static constexpr Vector zero;
	return *this != zero;
}

//-------------------------------------------------------------------------

constexpr Vector2 Vector<2>::Zero;
constexpr Vector2 Vector<2>::One{1.f};

//-------------------------------------------------------------------------

std::string Vector<2>::ToString() const
{
	std::ostringstream oss;
	oss << std::setprecision(4) << '(' << x << ", " << y << ')';
	return oss.str();
}

//-------------------------------------------------------------------------

float Vector<2>::Length() const
{
	return DirectX::XMVectorGetX(DirectX::XMVector2Length(XMVector(*this)));
}

float Vector<2>::LengthSq() const
{
	return DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(XMVector(*this)));
}

float Vector<2>::Dot(const Vector& other) const
{
	return DirectX::XMVectorGetX(DirectX::XMVector2Dot(XMVector(*this), XMVector(other)));
}

Vector<2> Vector<2>::Normalize() const
{
	return Vector(DirectX::XMVector2Normalize(XMVector(*this)));
}

Vector<2> Vector<2>::Transform(const Matrix3& matrix) const
{
	return Vector(DirectX::XMVector2Transform(XMVector(*this), XMMatrix(matrix)));
}

//-------------------------------------------------------------------------

bool Vector<2>::operator==(const Vector& other) const
{
	return DirectX::XMVector2Equal(XMVector(*this), XMVector(other));
}

bool Vector<2>::operator!=(const Vector& other) const
{
	return DirectX::XMVector2NotEqual(XMVector(*this), XMVector(other));
}

bool Vector<2>::operator<(const Vector& other) const
{
	return DirectX::XMVector2Less(XMVector(*this), XMVector(other));
}

bool Vector<2>::operator<=(const Vector& other) const
{
	return DirectX::XMVector2LessOrEqual(XMVector(*this), XMVector(other));
}

bool Vector<2>::operator>(const Vector& other) const
{
	return DirectX::XMVector2Greater(XMVector(*this), XMVector(other));
}

bool Vector<2>::operator>=(const Vector& other) const
{
	return DirectX::XMVector2GreaterOrEqual(XMVector(*this), XMVector(other));
}

//-------------------------------------------------------------------------
// Vector3
//-------------------------------------------------------------------------

Vector<3>& Vector<3>::operator=(const XMVector& xmVector)
{
	DirectX::XMStoreFloat3(reinterpret_cast<XMVector3*>(this), xmVector);
	return *this;
}

//-------------------------------------------------------------------------

Vector<3>::operator XMVector() const
{
	return DirectX::XMLoadFloat3(reinterpret_cast<const XMVector3*>(this));
}

Vector<3>::operator const XMVector3&() const
{
	return reinterpret_cast<const XMVector3&>(*this);
}

Vector<3>::operator bool() const
{
	static constexpr Vector zero;
	return *this != zero;
}

//-------------------------------------------------------------------------

constexpr Vector3 Vector<3>::Zero;
constexpr Vector3 Vector<3>::One{1.f};

constexpr Vector3 Vector<3>::Forward{0.f, 0.f, -1.f};
constexpr Vector3 Vector<3>::Backward{0.f, 0.f, 1.f};
constexpr Vector3 Vector<3>::Up{0.f, 1.f, 0.f};
constexpr Vector3 Vector<3>::Down{0.f, -1.f, 0.f};
constexpr Vector3 Vector<3>::Right{1.f, 0.f, 0.f};
constexpr Vector3 Vector<3>::Left{-1.f, 0.f, 0.f};

//-------------------------------------------------------------------------

std::string Vector<3>::ToString() const
{
	std::ostringstream oss;
	oss << std::setprecision(4) << '(' << x << ", " << y << ", " << z << ')';
	return oss.str();
}

//-------------------------------------------------------------------------

float Vector<3>::Length() const
{
	return DirectX::XMVectorGetX(DirectX::XMVector3Length(XMVector(*this)));
}

float Vector<3>::LengthSq() const
{
	return DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(XMVector(*this)));
}

float Vector<3>::Dot(const Vector& other) const
{
	return DirectX::XMVectorGetX(DirectX::XMVector3Dot(XMVector(*this), XMVector(other)));
}

Vector<3> Vector<3>::Cross(const Vector3& other) const
{
	return Vector(DirectX::XMVector3Cross(XMVector(*this), XMVector(other)));
}

Vector<3> Vector<3>::Normalize() const
{
	return Vector(DirectX::XMVector3Normalize(XMVector(*this)));
}

Vector<3> Vector<3>::Transform(const Matrix4& matrix) const
{
	return Vector(DirectX::XMVector3Transform(XMVector(*this), XMMatrix(matrix)));
}

Vector<3> Vector<3>::TransformNormal(const Matrix3& matrix) const
{
	return Vector(DirectX::XMVector3TransformNormal(XMVector(*this), XMMatrix(matrix)));
}

//-------------------------------------------------------------------------

bool Vector<3>::operator==(const Vector& other) const
{
	return DirectX::XMVector3Equal(XMVector(*this), XMVector(other));
}

bool Vector<3>::operator!=(const Vector& other) const
{
	return DirectX::XMVector3NotEqual(XMVector(*this), XMVector(other));
}

bool Vector<3>::operator<(const Vector& other) const
{
	return DirectX::XMVector3Less(XMVector(*this), XMVector(other));
}

bool Vector<3>::operator<=(const Vector& other) const
{
	return DirectX::XMVector3LessOrEqual(XMVector(*this), XMVector(other));
}

bool Vector<3>::operator>(const Vector& other) const
{
	return DirectX::XMVector3Greater(XMVector(*this), XMVector(other));
}

bool Vector<3>::operator>=(const Vector& other) const
{
	return DirectX::XMVector3GreaterOrEqual(XMVector(*this), XMVector(other));
}

//-------------------------------------------------------------------------
// Vector4
//-------------------------------------------------------------------------

Vector<4>& Vector<4>::operator=(const XMVector& xmVector)
{
	DirectX::XMStoreFloat4(reinterpret_cast<XMVector4*>(this), xmVector);
	return *this;
}

//-------------------------------------------------------------------------

Vector<4>::operator XMVector() const
{
	return DirectX::XMLoadFloat4(reinterpret_cast<const XMVector4*>(this));
}

Vector<4>::operator const XMVector4&() const
{
	return reinterpret_cast<const XMVector4&>(*this);
}

Vector<4>::operator bool() const
{
	static constexpr Vector zero;
	return *this != zero;
}

//-------------------------------------------------------------------------

constexpr Vector4 Vector<4>::Zero;
constexpr Vector4 Vector<4>::One{1.f};

//-------------------------------------------------------------------------

std::string Vector<4>::ToString() const
{
	std::ostringstream oss;
	oss << std::setprecision(4) << '(' << x << ", " << y << ", " << z << ", " << w << ')';
	return oss.str();
}

//-------------------------------------------------------------------------

float Vector<4>::Length() const
{
	return DirectX::XMVectorGetX(DirectX::XMVector4Length(XMVector(*this)));
}

float Vector<4>::LengthSq() const
{
	return DirectX::XMVectorGetX(DirectX::XMVector4LengthSq(XMVector(*this)));
}

float Vector<4>::Dot(const Vector& other) const
{
	return DirectX::XMVectorGetX(DirectX::XMVector4Dot(XMVector(*this), XMVector(other)));
}

Vector<4> Vector<4>::Normalize() const
{
	return Vector(DirectX::XMVector4Normalize(XMVector(*this)));
}

Vector<4> Vector<4>::Transform(const Matrix4& matrix) const
{
	return Vector(DirectX::XMVector4Transform(XMVector(*this), XMMatrix(matrix)));
}

//-------------------------------------------------------------------------

bool Vector<4>::operator==(const Vector& other) const
{
	return DirectX::XMVector4Equal(XMVector(*this), XMVector(other));
}

bool Vector<4>::operator!=(const Vector& other) const
{
	return DirectX::XMVector4NotEqual(XMVector(*this), XMVector(other));
}

bool Vector<4>::operator<(const Vector& other) const
{
	return DirectX::XMVector4Less(XMVector(*this), XMVector(other));
}

bool Vector<4>::operator<=(const Vector& other) const
{
	return DirectX::XMVector4LessOrEqual(XMVector(*this), XMVector(other));
}

bool Vector<4>::operator>(const Vector& other) const
{
	return DirectX::XMVector4Greater(XMVector(*this), XMVector(other));
}

bool Vector<4>::operator>=(const Vector& other) const
{
	return DirectX::XMVector4GreaterOrEqual(XMVector(*this), XMVector(other));
}
} // namespace library::math
