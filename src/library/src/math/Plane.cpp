#include "StdAfx.h"
#include "library/Math/Plane.h"

#include "library/Math/Matrix.h"

namespace library::math
{
Plane::Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3) : Plane()
{
	DirectX::XMStoreFloat4(
		reinterpret_cast<XMVector4*>(this),
		DirectX::XMPlaneFromPoints(XMVector(p1), XMVector(p2), XMVector(p3)));
}

Plane::Plane(const Vector3& point, const Vector3& normal) : Plane()
{
	DirectX::XMStoreFloat4(
		reinterpret_cast<XMVector4*>(this),
		DirectX::XMPlaneFromPointNormal(XMVector(point), XMVector(normal)));
}

Plane& Plane::operator=(const XMVector& xmVector)
{
	DirectX::XMStoreFloat4(reinterpret_cast<XMVector4*>(this), xmVector);
	return *this;
}

//-------------------------------------------------------------------------

Plane::operator XMVector() const
{
	return DirectX::XMLoadFloat4(reinterpret_cast<const XMVector4*>(this));
}

Plane::operator const XMVector4&() const
{
	return reinterpret_cast<const XMVector4&>(*this);
}

Plane::operator bool() const
{
	static constexpr Plane zero;
	return *this != zero;
}

//-------------------------------------------------------------------------

Plane Plane::Transform(const Matrix4& matrix) const
{
	return Plane(DirectX::XMPlaneTransform(XMVector(*this), XMMatrix(matrix)));
}

float Plane::Dot(const Vector4& vector) const
{
	return DirectX::XMVectorGetX(DirectX::XMPlaneDot(XMVector(*this), XMVector(vector)));
}

float Plane::DotCoordinate(const Vector3& point) const
{
	return DirectX::XMVectorGetX(DirectX::XMPlaneDotCoord(XMVector(*this), XMVector(point)));
}

float Plane::DotNormal(const Vector3& normal) const
{
	return DirectX::XMVectorGetX(DirectX::XMPlaneDotNormal(XMVector(*this), XMVector(normal)));
}

Plane Plane::Normalize() const
{
	return Plane(DirectX::XMPlaneNormalize(XMVector(*this)));
}

//-------------------------------------------------------------------------

Ray Plane::Intersect(const Plane& other) const
{
	const auto direction = normal.Cross(other.normal);
	const auto position = direction.Cross(normal * other.d - other.normal * d) / direction.LengthSq();

	return Ray(position, direction);
}

Vector3 Plane::Intersect(const Ray& ray) const
{
	const float value = -(d + normal.Dot(ray.position)) / normal.Dot(ray.direction);

	return ray.position + ray.direction * value;
}

//-------------------------------------------------------------------------

bool Plane::operator==(const Plane& other) const
{
	return DirectX::XMPlaneEqual(XMVector(*this), XMVector(other));
}

bool Plane::operator!=(const Plane& other) const
{
	return DirectX::XMPlaneNotEqual(XMVector(*this), XMVector(other));
}
} // namespace library::math
