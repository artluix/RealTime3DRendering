#pragma once
#include <chrono>

namespace library
{
    // use std::chrono types
    using ClockType = std::chrono::high_resolution_clock;
    using TimePointType = ClockType::time_point;
    using DurationType = ClockType::duration;

    /////////////////////////////////////////////////////////////////

    struct TimeValue
    {
    public:
        explicit TimeValue() = default;
        explicit TimeValue(const DurationType& duration); // from duration
        explicit TimeValue(const TimePointType& timePoint); // from time point

        long long GetNanoseconds() const;   // 10 ^ -9 seconds
        long long GetMicroseconds() const;  // 10 ^ -6 seconds
        long long GetMilliseconds() const;  // 10 ^ -3 seconds
        long long GetSeconds() const;       // 1 second
        int GetMinutes() const;             // 60 seconds
        int GetHours() const;               // 3600 seconds

    private:
        DurationType m_duration;
    }; 

    /////////////////////////////////////////////////////////////////

    struct Time
    {
        explicit Time() = default;
        explicit Time(const TimeValue& _elapsed, const TimeValue& _total);

        TimeValue total;
        TimeValue elapsed;
    };
}

