#pragma once
#include <chrono>

namespace library
{
	namespace animation
	{
		using Duration = std::chrono::duration<float>;
		using TimePoint = std::chrono::time_point<std::chrono::steady_clock, Duration>;
	} // namespace animation
} // namespace library
