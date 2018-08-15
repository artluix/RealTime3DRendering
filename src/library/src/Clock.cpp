#include "library/Clock.h"

namespace library
{
    Clock::Clock()
    {
        Reset();
    }

    void Clock::Reset()
    {
        m_startTimePoint = m_currentTimePoint = MonotonicClock::now();
    }

    void Clock::UpdateTime(Time& time)
    {
        auto now = MonotonicClock::now();

        auto elapsed = now - m_currentTimePoint;
        auto total = now - m_startTimePoint;

        time.elapsed = TimeValue(elapsed);
        time.total = TimeValue(total);

        m_currentTimePoint = now;
    }
}
