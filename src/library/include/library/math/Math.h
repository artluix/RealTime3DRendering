#pragma once
#include "library/Math/Color.h"
#include "library/Math/Matrix.h"
#include "library/Math/Plane.h"
#include "library/Math/Quaternion.h"
#include "library/Math/Ray.h"
#include "library/Math/Transform.h"
#include "library/Math/Vector.h"

namespace library::math
{
inline constexpr float Pi = 3.141592654f;
inline constexpr float Pi_2 = Pi * 2;
inline constexpr float Pi_Div_2 = Pi / 2;
inline constexpr float Pi_Div_4 = Pi / 4;

inline constexpr float Radian = 180.f / Pi;

//-------------------------------------------------------------------------

constexpr float ToRadians(const float degrees)
{
	return degrees / Radian;
}
constexpr float ToDegrees(const float radians)
{
	return radians * Radian;
}

//-------------------------------------------------------------------------

template <typename T>
constexpr T Min(const T& lhs, const T& rhs)
{
	return (lhs < rhs) ? lhs : rhs;
}

template <typename T>
constexpr T Max(const T& lhs, const T& rhs)
{
	return (lhs > rhs) ? lhs : rhs;
}

template <typename T>
constexpr T Clamp(const T& x, const T& min, const T& max)
{
	return Min(Max(x, min), max);
}

template <typename T>
constexpr T Lerp(const T& lhs, const T& rhs, const float factor)
{
	return lhs + factor * (rhs - lhs);
}
} // namespace library::math
