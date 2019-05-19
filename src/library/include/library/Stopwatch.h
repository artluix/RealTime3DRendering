#pragma once
#include "library/Time.h"

namespace library
{
class Stopwatch
{
public:
	Stopwatch();

	void Reset();
	void UpdateTime(Time& time);

private:
	TimePoint m_startTimePoint;
	TimePoint m_currentTimePoint;
};
} // namespace library
