#include "StdAfx.h"
#include "library/Stopwatch.h"

namespace library
{
Stopwatch::Stopwatch()
{
	Reset();
}

void Stopwatch::Reset()
{
	m_startTimePoint = m_currentTimePoint = Clock::now();
}

void Stopwatch::UpdateTime(Time& time)
{
	const auto now = Clock::now();

	const auto elapsed = now - m_currentTimePoint;
	const auto total = now - m_startTimePoint;

	time.elapsed = TimeValue(elapsed);
	time.total = TimeValue(total);

	m_currentTimePoint = now;
}
} // namespace library
