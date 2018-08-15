#pragma once
#include <chrono>

namespace library
{
    using MonotonicClock = std::chrono::steady_clock;

    struct TimeValue
    {
    public:
        explicit TimeValue() = default;
        explicit TimeValue(const MonotonicClock::duration& duration)
            : m_duration(duration)
        {
        }

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

        MonotonicClock::duration m_duration;
    }; 

    /////////////////////////////////////////////////////////////////

    struct Time
    {
        explicit Time() = default;
        explicit Time(const TimeValue& _elapsed, const TimeValue& _total)
            : elapsed(_elapsed)
            , total(_total)
        {
        }

        TimeValue total;
        TimeValue elapsed;
    };
}
