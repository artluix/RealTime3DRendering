#include "StdAfx.h"
#include "library/Math/Plane.h"

#include "library/Math/Matrix.h"

namespace library::math
{
Plane::Plane(const Vector3& p1, const Vector3& p2, const Vector3& p3)
	: Plane(dx::XMPlaneFromPoints(dx::VECTOR(p1), dx::VECTOR(p2), dx::VECTOR(p3)))
{}

Plane::Plane(const Vector3& point, const Vector3& normal)
	: Plane(dx::XMPlaneFromPointNormal(dx::VECTOR(point), dx::VECTOR(normal)))
{}

//-------------------------------------------------------------------------

Plane Plane::Transform(const Matrix4& matrix) const
{
	return Plane(dx::XMPlaneTransform(dx::VECTOR(*this), dx::MATRIX(matrix)));
}

float Plane::Dot(const Vector4& vector) const
{
	return dx::XMVectorGetX(dx::XMPlaneDot(dx::VECTOR(*this), dx::VECTOR(vector)));
}

float Plane::DotCoordinate(const Vector3& point) const
{
	return dx::XMVectorGetX(dx::XMPlaneDotCoord(dx::VECTOR(*this), dx::VECTOR(point)));
}

float Plane::DotNormal(const Vector3& normal) const
{
	return dx::XMVectorGetX(dx::XMPlaneDotNormal(dx::VECTOR(*this), dx::VECTOR(normal)));
}

Plane Plane::Normalize() const
{
	return Plane(dx::XMPlaneNormalize(dx::VECTOR(*this)));
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
} // namespace library::math
