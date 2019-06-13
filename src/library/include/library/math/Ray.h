#pragma once
#include "library/Math/Vector.h"

namespace library::math
{
struct Ray
{
	Vector3 position;
	Vector3 direction;

	Ray(const Vector3& position, const Vector3& direction)
		: position(position)
		, direction(direction)
	{}
};
} // namespace library::math
