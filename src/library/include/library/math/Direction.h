#pragma once
#include "library/Math/Vector.h"

namespace library::math
{
struct Direction
{
	static constexpr Vector3 Forward	{ 0, 0, -1 };
	static constexpr Vector3 Backward	{ 0, 0, 1 };
	static constexpr Vector3 Up			{ 0, 1, 0 };
	static constexpr Vector3 Down		{ 0, -1, 0 };
	static constexpr Vector3 Right		{ 1, 0, 0 };
	static constexpr Vector3 Left		{ -1, 0, 0 };
};
} // namespace library::math
