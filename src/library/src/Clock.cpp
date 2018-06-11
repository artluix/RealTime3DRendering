#include "library/Clock.h"


namespace library
{
    Clock::Clock()
    {
        Reset();
    }

    TimeValue Clock::GetStartTime() const
    {
        return TimeValue(m_startTime);
    }

    TimeValue Clock::GetCurrentTime() const
    {
        return TimeValue(m_currentTime);
    }

    TimeValue Clock::GetLastTime() const
    {
        return TimeValue(m_lastTime);
    }

    void Clock::Reset()
    {
        GetTime(m_startTime);
        m_currentTime = m_startTime;
        m_lastTime = m_startTime;
    }

    void Clock::GetTime(TimePointType& time) const
    {
        time = ClockType::now();
    }

    void Clock::UpdateTime(Time& time)
    {
        GetTime(m_currentTime);

        auto total = m_currentTime - m_startTime;
        auto elapsed = m_currentTime - m_startTime;

        time.elapsed = TimeValue(elapsed);
        time.total = TimeValue(total);

        m_lastTime = m_currentTime;
    }
}
