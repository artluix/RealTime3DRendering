#pragma once
#include "library/Time.h"
#include "library/NonCopyable.hpp"

namespace library
{
    class Clock : public NonCopyable<Clock>
    {
    public:
        explicit Clock();

        void Reset();
        void UpdateTime(Time& time);

    private:
        MonotonicClock::time_point m_startTimePoint;
        MonotonicClock::time_point m_currentTimePoint;
    };
}

