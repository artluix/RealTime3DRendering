#pragma once
#include <chrono>

namespace library
{
	using Clock = std::chrono::steady_clock;
	using TimePoint = Clock::time_point;
	using Duration = Clock::duration;

	//-------------------------------------------------------------------------

	struct TimeValue
	{
	public:
		explicit TimeValue() = default;
		explicit TimeValue(const Duration& duration)
			: m_duration(duration)
		{
		}

		Duration GetDuration() const { return m_duration; }

		template <typename T>
		T GetNanoseconds() const { return GetAs<T, std::chrono::nanoseconds::period>(); }

		template <typename T>
		T GetMicroseconds() const { return GetAs<T, std::chrono::microseconds::period>(); }

		template <typename T>
		T GetMilliseconds() const { return GetAs<T, std::chrono::milliseconds::period>(); }

		template <typename T>
		T GetSeconds() const { return GetAs<T, std::chrono::seconds::period>(); }

		template <typename T>
		T GetMinutes() const { return GetAs<T, std::chrono::minutes::period>(); }

		template <typename T>
		T GetHours() const { return GetAs<T, std::chrono::hours::period>(); }

	private:
		template <typename T, typename Period>
		T GetAs() const
		{
			return std::chrono::duration_cast<std::chrono::duration<T, Period>>(m_duration).count();
		}

		Duration m_duration;
	};

	//-------------------------------------------------------------------------

	struct Time
	{
		explicit Time() = default;
		explicit Time(const TimeValue& _elapsed, const TimeValue& _total)
			: total(_total)
			, elapsed(_elapsed)
		{
		}

		TimeValue total;
		TimeValue elapsed;
	};
} // namespace library
