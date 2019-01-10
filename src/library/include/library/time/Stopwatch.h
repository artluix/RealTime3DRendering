#pragma once
#include "library/time/Time.h"

namespace library
{
	class Stopwatch
	{
	public:
		explicit Stopwatch();

		void Reset();
		void UpdateTime(Time& time);

	private:
		TimePoint m_startTimePoint;
		TimePoint m_currentTimePoint;
	};
} // namespace library
