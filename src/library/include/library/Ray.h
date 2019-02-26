#pragma once
#include "math/Vector.h"

namespace library
{
	struct Ray
	{
		explicit Ray(const math::Vector3& position, const math::Vector3& direction)
			: position(position)
			, direction(direction)
		{
		}

		math::Vector3 position;
		math::Vector3 direction;
	};
} // namespace library
