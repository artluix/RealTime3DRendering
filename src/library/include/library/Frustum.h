#pragma once
#include "library/Math/Matrix.h"
#include "library/math/Plane.h"

#include <array>

namespace library
{
class Frustum
{
public:
	struct Plane
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

		static bool IsValid(const Type t);
	};

	//-------------------------------------------------------------------------

	struct Corner
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

		static bool IsValid(const Type t);
	};

	//-------------------------------------------------------------------------

	using PlaneFixedArray = std::array<math::Plane, Plane::Count>;
	using CornerFixedArray = std::array<math::Vector3, Corner::Count>;

	//-------------------------------------------------------------------------

	explicit Frustum(const math::Matrix4& projectionMatrix = math::Matrix4::Identity);

	const math::Plane& GetPlane(const Plane::Type planeType) const;
	const math::Vector3& GetCorner(const Corner::Type cornerType) const;

	const PlaneFixedArray& GetPlanes() const { return m_planes; }
	const CornerFixedArray& GetCorners() const { return m_corners; }

	const math::Matrix4& GetProjectionMatrix() const { return m_projectionMatrix; }
	void SetProjectionMatrix(const math::Matrix4& projectionMatrix);

private:
	math::Matrix4 m_projectionMatrix;
	PlaneFixedArray m_planes;
	CornerFixedArray m_corners;
};
} // namespace library
