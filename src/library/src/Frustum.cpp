#include "StdAfx.h"
#include "library/Frustum.h"

namespace library
{
	namespace
	{
		Ray ComputeIntersectionLine(const math::Vector4& plane1, const math::Vector4& plane2)
		{
			const math::Vector3 plane1Vec3(plane1);
			const math::Vector3 plane2Vec3(plane2);

			const auto direction = plane1Vec3.Cross(plane2Vec3);
			const auto position =
				-direction.Cross(plane1Vec3 * plane2.w - plane2Vec3 * plane1.w) / direction.LengthSq();

			return Ray(position, direction);
		}

		math::Vector3 ComputeIntersection(const math::Vector4& plane, const Ray& ray)
		{
			const math::Vector3 planeVec3(plane);

			const float value = -(plane.w + planeVec3.Dot(ray.position)) / planeVec3.Dot(ray.direction);

			return ray.position + ray.direction * value;
		}
	}

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

	Frustum::Frustum(const math::Matrix4& matrix /*= math::Matrix4::Identity*/)
	{
		SetMatrix(matrix);
	}

	const math::Vector4& Frustum::GetPlane(const Plane::Type planeType)
	{
		assert(Plane::IsValid(planeType));
		return m_planes[planeType];
	}

	const math::Vector3& Frustum::GetCorner(const Corner::Type cornerType)
	{
		assert(Corner::IsValid(cornerType));
		return m_corners[cornerType];
	}

	void Frustum::SetMatrix(const math::Matrix4& matrix)
	{
		m_matrix = matrix;

		m_planes[Plane::Near] = -matrix.GetCol(2);
		m_planes[Plane::Far] = -matrix.GetCol(3) + matrix.GetCol(2);
		m_planes[Plane::Left] = -matrix.GetCol(3) - matrix.GetCol(0);
		m_planes[Plane::Right] = -matrix.GetCol(3) + matrix.GetCol(0);
		m_planes[Plane::Top] = -matrix.GetCol(3) + matrix.GetCol(1);
		m_planes[Plane::Bottom] = -matrix.GetCol(3) - matrix.GetCol(1);

		auto normalizePred = [](math::Vector4& plane)
		{
			plane = plane.PlaneNormalize();
		};
		std::for_each(m_planes.begin(), m_planes.end(), normalizePred);

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
