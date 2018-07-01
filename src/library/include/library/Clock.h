#pragma once
#include "library/Time.h"
#include "library/NonCopyable.hpp"

namespace library
{
    class Clock : public NonCopyable<Clock>
    {
    public:
        explicit Clock();

        TimeValue GetStartTime() const;
        TimeValue GetCurrentTime() const;
        TimeValue GetLastTime() const;

        void Reset();
        void UpdateTime(Time& time);

    private:
        void GetTime(TimePointType& time) const;

    private:
        TimePointType m_startTime;
        TimePointType m_currentTime;
        TimePointType m_lastTime;
    };
}

