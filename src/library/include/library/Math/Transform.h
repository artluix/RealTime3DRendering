#pragma once
#include "library/Math/Quaternion.h"
#include "library/Math/Vector.h"

namespace library::math
{
struct Transform
{
	Quaternion rotation = Quaternion::Identity;
	Vector3 scaling = Vector3::One;
	Vector3 position = Vector3::Zero;
};
} // namespace library::math
