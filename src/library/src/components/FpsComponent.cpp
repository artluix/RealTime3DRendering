#include "StdAfx.h"
#include "library/components/FpsComponent.h"

namespace library
{
	namespace
	{
		constexpr auto k_fpsMeasureInterval = std::chrono::seconds(1);
	}

	FpsComponent::FpsComponent(const Application& app)
		: TextComponent(app)
		, m_frameCount(0)
		, m_frameRate(0)
		, m_timeAccumulator(Duration::zero())
	{
	}

	void FpsComponent::Update(const Time& time)
	{
		const auto elapsed = time.elapsed.GetDuration();
		if (m_timeAccumulator >= k_fpsMeasureInterval)
		{
			m_frameRate = m_frameCount;

			m_timeAccumulator = Duration::zero();
			m_frameCount = 0;
		}
		else
		{
			m_timeAccumulator += elapsed;
		}

		m_frameCount++;

		std::wostringstream woss;
		woss << std::setprecision(4) <<
			L"Frame Rate: " << m_frameRate << "\n" <<
			L"Total Elapsed Time: " << time.total.GetSeconds<float>();

		SetText(woss.str());
	}
} // namespace library
