#pragma once
#include "library/Math/MathFwd.h"
#include "library/Math/MathUtils.h"

#include <sstream>

namespace library::math
{
template<typename T>
struct IntervalType
{
	T min, max;

	constexpr IntervalType(const T a, const T b) : min(Min(a, b)), max(Max(a, b)) {}

	constexpr T GetLength() const { return (max - min); }

	constexpr bool Contains(const T value) const { return (value >= min && value <= max); }
	constexpr bool Contains(const Interval& other) const { return (other.min >= min && other.max <= max); }

	constexpr T Clamp(const T& value) const { return Clamp(value, min, max); }

	//-------------------------------------------------------------------------

	std::string ToString() const
	{
		std::ostringstream oss;
		oss << std::setprecision(4) << '(' << min << ", " << max << ')';
		return oss.str();
	}
};

inline constexpr Interval UnitInterval(0.f, 1.f);

} // namespace library::math
