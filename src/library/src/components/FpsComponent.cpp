#include "StdAfx.h"
#include "library/Components/FpsComponent.h"

namespace library
{
	namespace
	{
		constexpr auto k_fpsMeasureInterval = std::chrono::seconds(1);
	}

	//-------------------------------------------------------------------------

	FpsComponent::FpsComponent()
		: m_framesCount(0)
		, m_frameRate(0)
		, m_timeAccumulator(Duration::zero())
	{
	}

	FpsComponent::~FpsComponent() = default;

	//-------------------------------------------------------------------------

	void FpsComponent::Update(const Time& time)
	{
		const auto elapsed = time.elapsed.GetDuration();
		if (m_timeAccumulator >= k_fpsMeasureInterval)
		{
			m_frameRate = m_framesCount;

			m_timeAccumulator = Duration::zero();
			m_framesCount = 0;
		}
		else
		{
			m_timeAccumulator += elapsed;
		}

		m_framesCount++;

		std::wostringstream woss;
		woss << std::setprecision(4) <<
			L"Frame Rate: " << m_frameRate << "\n" <<
			L"Total Elapsed Time: " << time.total.GetSeconds();

		SetText(woss.str());

		TextComponent::Update(time);
	}
} // namespace library
