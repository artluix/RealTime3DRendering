#pragma once
#include "library/Math/MathConstants.h"

#include <type_traits>

namespace library::math
{
constexpr float ToRadians(const float degrees) { return degrees / Radian; }
constexpr float ToDegrees(const float radians) { return radians * Radian; }

//-------------------------------------------------------------------------

template <typename T>
constexpr std::enable_if_t<std::is_arithmetic_v<T>, T>
Min(const T& lhs, const T& rhs) { return (lhs < rhs) ? lhs : rhs; }

template <typename T>
constexpr std::enable_if_t<std::is_arithmetic_v<T>, T>
Max(const T& lhs, const T& rhs) { return (lhs > rhs) ? lhs : rhs; }

template <typename T>
constexpr std::enable_if_t<std::is_arithmetic_v<T>, T>
Clamp(const T& x, const T& min, const T& max) { return Min(Max(x, min), max); }

template <typename T>
constexpr std::enable_if_t<std::is_arithmetic_v<T>, T>
Lerp(const T& lhs, const T& rhs, const float factor) { return lhs + factor * (rhs - lhs); }

template <typename T>
constexpr std::enable_if_t<std::is_arithmetic_v<T>, T>
Abs(const T& val) { return (val > 0) ? val : -val; }

} // namespace library::math
