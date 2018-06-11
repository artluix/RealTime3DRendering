#include "library/Time.h"


namespace library
{
    TimeValue::TimeValue(const DurationType& duration)
        : m_duration(duration)
    {}

    TimeValue::TimeValue(const TimePointType& timePoint)
        : m_duration(timePoint.time_since_epoch())
    {}

    long long TimeValue::GetNanoseconds() const
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(m_duration).count();
    }

    long long TimeValue::GetMicroseconds() const
    {
        return std::chrono::duration_cast<std::chrono::microseconds>(m_duration).count();
    }

    long long TimeValue::GetMilliseconds() const
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(m_duration).count();
    }

    long long TimeValue::GetSeconds() const
    {
        return std::chrono::duration_cast<std::chrono::seconds>(m_duration).count();
    }

    int TimeValue::GetMinutes() const
    {
        return std::chrono::duration_cast<std::chrono::minutes>(m_duration).count();
    }

    int TimeValue::GetHours() const
    {
        return std::chrono::duration_cast<std::chrono::hours>(m_duration).count();
    }

    /////////////////////////////////////////////////////////////

    Time::Time(const TimeValue& _elapsed, const TimeValue& _total)
        : elapsed(_elapsed)
        , total(_total)
    {}
}
