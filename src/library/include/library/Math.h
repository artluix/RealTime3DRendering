#pragma once
#include "library/Vector.h"
#include "library/Matrix.h"

namespace library
{
	namespace math
	{

		//-------------------------------------------------------------------------
		// constants
		//-------------------------------------------------------------------------

		namespace constants
		{
			inline constexpr float Pi = 3.141592654f;
			inline constexpr float Pi_2 = 2 * Pi;
			inline constexpr float Pi_Div_2 = Pi / 2;
			inline constexpr float Pi_Div_4 = Pi / 4;

			inline constexpr float Radian = 180.f / Pi;
		} // namespace constants

		//-------------------------------------------------------------------------
		// functions
		//-------------------------------------------------------------------------

		inline constexpr float ToRadians(const float degrees) { return degrees / constants::Radian; }
		inline constexpr float ToDegrees(const float radians) { return radians * constants::Radian; }

		template <typename T>
		inline constexpr T Min(const T& lhs, const T& rhs)
		{
			return (lhs < rhs) ? lhs : rhs;
		}

		template <typename T>
		inline constexpr T Max(const T& lhs, const T& rhs)
		{
			return (lhs > rhs) ? lhs : rhs;
		}

		template <typename T>
		inline constexpr T Clamp(const T& x, const T& min, const T& max)
		{
			return Min(Max(x, min), max);
		}

	} // namespace math
} // namespace library
