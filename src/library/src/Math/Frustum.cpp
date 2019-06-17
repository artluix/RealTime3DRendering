#include "StdAfx.h"
#include "library/Math/Frustum.h"

#include "library/Math/Ray.h"

namespace library::math
{

namespace
{
Ray ComputeIntersectionLine(const Plane& plane1, const Plane& plane2)
{
	const auto direction = plane1.normal.Cross(plane2.normal);
	const auto position =
		direction.Cross(plane1.normal * plane2.d - plane2.normal * plane1.d) / direction.LengthSq();

	return Ray(position, direction);
}

Vector3 ComputeIntersection(const Plane& plane, const Ray& ray)
{
	const float value = -(plane.d + plane.normal.Dot(ray.position)) / plane.normal.Dot(ray.direction);

	return ray.position + ray.direction * value;
}
} // namespace

//-------------------------------------------------------------------------

Frustum::Frustum(const Matrix4& projectionMatrix /*= Matrix4::Identity*/)
{
	SetProjectionMatrix(projectionMatrix);
}

//-------------------------------------------------------------------------

const Plane& Frustum::GetPlane(const PlaneType planeType) const
{
	return m_planes[planeType];
}

const Vector3& Frustum::GetCorner(const CornerType cornerType) const
{
	return m_corners[cornerType];
}

void Frustum::SetProjectionMatrix(const Matrix4& projectionMatrix)
{
	m_projectionMatrix = projectionMatrix;

	m_planes[PlaneType::Near] = Plane(-projectionMatrix.GetCol(2));
	m_planes[PlaneType::Far] = Plane(-projectionMatrix.GetCol(3) + projectionMatrix.GetCol(2));
	m_planes[PlaneType::Left] = Plane(-projectionMatrix.GetCol(3) - projectionMatrix.GetCol(0));
	m_planes[PlaneType::Right] = Plane(-projectionMatrix.GetCol(3) + projectionMatrix.GetCol(0));
	m_planes[PlaneType::Top] = Plane(-projectionMatrix.GetCol(3) + projectionMatrix.GetCol(1));
	m_planes[PlaneType::Bottom] = Plane(-projectionMatrix.GetCol(3) - projectionMatrix.GetCol(1));

	// normalize planes
	for (auto& plane : m_planes)
	{
		plane = plane.Normalize();
	}

	auto ray = ComputeIntersectionLine(GetPlane(PlaneType::Near), GetPlane(PlaneType::Left));
	m_corners[CornerType::NearTopLeft] = ComputeIntersection(GetPlane(PlaneType::Top), ray);
	m_corners[CornerType::NearBottomLeft] = ComputeIntersection(GetPlane(PlaneType::Bottom), ray);

	ray = ComputeIntersectionLine(GetPlane(PlaneType::Near), GetPlane(PlaneType::Right));
	m_corners[CornerType::NearTopRight] = ComputeIntersection(GetPlane(PlaneType::Top), ray);
	m_corners[CornerType::NearBottomRight] = ComputeIntersection(GetPlane(PlaneType::Bottom), ray);

	ray = ComputeIntersectionLine(GetPlane(PlaneType::Far), GetPlane(PlaneType::Left));
	m_corners[CornerType::FarTopLeft] = ComputeIntersection(GetPlane(PlaneType::Top), ray);
	m_corners[CornerType::FarBottomLeft] = ComputeIntersection(GetPlane(PlaneType::Bottom), ray);

	ray = ComputeIntersectionLine(GetPlane(PlaneType::Far), GetPlane(PlaneType::Right));
	m_corners[CornerType::FarTopRight] = ComputeIntersection(GetPlane(PlaneType::Top), ray);
	m_corners[CornerType::FarBottomRight] = ComputeIntersection(GetPlane(PlaneType::Bottom), ray);
}
} // namespace library
