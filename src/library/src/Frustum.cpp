#include "StdAfx.h"
#include "library/Frustum.h"

#include "library/math/Ray.h"

namespace library
{

namespace
{
math::Ray ComputeIntersectionLine(const math::Plane& plane1, const math::Plane& plane2)
{
	const auto direction = plane1.normal.Cross(plane2.normal);
	const auto position =
		-direction.Cross(plane1.normal * plane2.d - plane2.normal * plane1.d) / direction.LengthSq();

	return math::Ray(position, direction);
}

math::Vector3 ComputeIntersection(const math::Plane& plane, const math::Ray& ray)
{
	const float value = -(plane.d + plane.normal.Dot(ray.position)) / plane.normal.Dot(ray.direction);

	return ray.position + ray.direction * value;
}
} // namespace

//-------------------------------------------------------------------------

bool Frustum::Plane::IsValid(const Type t)
{
	return t < Count;
}

bool Frustum::Corner::IsValid(const Type t)
{
	return t < Count;
}

//-------------------------------------------------------------------------

Frustum::Frustum(const math::Matrix4& projectionMatrix /*= math::Matrix4::Identity*/)
{
	SetProjectionMatrix(projectionMatrix);
}

//-------------------------------------------------------------------------

const math::Plane& Frustum::GetPlane(const Plane::Type planeType) const
{
	assert(Plane::IsValid(planeType));
	return m_planes[planeType];
}

const math::Vector3& Frustum::GetCorner(const Corner::Type cornerType) const
{
	assert(Corner::IsValid(cornerType));
	return m_corners[cornerType];
}

void Frustum::SetProjectionMatrix(const math::Matrix4& projectionMatrix)
{
	m_projectionMatrix = projectionMatrix;

	m_planes[Plane::Near] = math::Plane(-projectionMatrix.GetCol(2));
	m_planes[Plane::Far] = math::Plane(-projectionMatrix.GetCol(3) + projectionMatrix.GetCol(2));
	m_planes[Plane::Left] = math::Plane(-projectionMatrix.GetCol(3) - projectionMatrix.GetCol(0));
	m_planes[Plane::Right] = math::Plane(-projectionMatrix.GetCol(3) + projectionMatrix.GetCol(0));
	m_planes[Plane::Top] = math::Plane(-projectionMatrix.GetCol(3) + projectionMatrix.GetCol(1));
	m_planes[Plane::Bottom] = math::Plane(-projectionMatrix.GetCol(3) - projectionMatrix.GetCol(1));

	// normalize planes
	for (auto& plane : m_planes)
	{
		plane = plane.Normalize();
	}

	auto ray = ComputeIntersectionLine(GetPlane(Plane::Near), GetPlane(Plane::Left));
	m_corners[Corner::NearTopLeft] = ComputeIntersection(GetPlane(Plane::Top), ray);
	m_corners[Corner::NearBottomLeft] = ComputeIntersection(GetPlane(Plane::Bottom), ray);

	ray = ComputeIntersectionLine(GetPlane(Plane::Near), GetPlane(Plane::Right));
	m_corners[Corner::NearTopRight] = ComputeIntersection(GetPlane(Plane::Top), ray);
	m_corners[Corner::NearBottomRight] = ComputeIntersection(GetPlane(Plane::Bottom), ray);

	ray = ComputeIntersectionLine(GetPlane(Plane::Far), GetPlane(Plane::Left));
	m_corners[Corner::FarTopLeft] = ComputeIntersection(GetPlane(Plane::Top), ray);
	m_corners[Corner::FarBottomLeft] = ComputeIntersection(GetPlane(Plane::Bottom), ray);

	ray = ComputeIntersectionLine(GetPlane(Plane::Far), GetPlane(Plane::Right));
	m_corners[Corner::FarTopRight] = ComputeIntersection(GetPlane(Plane::Top), ray);
	m_corners[Corner::FarBottomRight] = ComputeIntersection(GetPlane(Plane::Bottom), ray);
}
} // namespace library
