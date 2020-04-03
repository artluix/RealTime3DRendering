#pragma once
#include "library/Math/Matrix.h"
#include "library/Math/Plane.h"

namespace library::math
{
class Frustum
{
	struct _Plane // just for scoping
	{
		enum Type : unsigned
		{
			Near = 0,
			Far,
			Left,
			Right,
			Top,
			Bottom,

			//# Count
			Count
		};
	};

	//-------------------------------------------------------------------------

	struct _Corner // just for scoping
	{
		enum Type : unsigned
		{
			NearTopLeft = 0,
			NearTopRight,
			NearBottomRight,
			NearBottomLeft,

			FarTopLeft,
			FarTopRight,
			FarBottomRight,
			FarBottomLeft,

			//# Count
			Count
		};
	};

public:
	using PlaneType = _Plane::Type;
	using CornerType = _Corner::Type;

	using PlaneArray = Array<Plane, PlaneType::Count>;
	using CornerArray = Array<Vector3, CornerType::Count>;

	//-------------------------------------------------------------------------

	explicit Frustum(const Matrix4& projectionMatrix = Matrix4::Identity);

	const math::Plane& GetPlane(const PlaneType planeType) const;
	const Vector3& GetCorner(const CornerType cornerType) const;

	const PlaneArray& GetPlanes() const { return m_planes; }
	const CornerArray& GetCorners() const { return m_corners; }

	const Matrix4& GetProjectionMatrix() const { return m_projectionMatrix; }
	void SetProjectionMatrix(const Matrix4& projectionMatrix);

private:
	Matrix4 m_projectionMatrix;
	PlaneArray m_planes;
	CornerArray m_corners;
};
} // namespace library::math
