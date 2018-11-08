#include "StdAfx.h"
#include "library/components/FpsComponent.h"

namespace library
{

	namespace
	{
		const auto k_fpsMeasureInterval = std::chrono::seconds(1);
	}

	namespace components
	{
		FpsComponent::FpsComponent(const Application& app)
			: Class(app)
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
				L"Frame Rate: " << m_frameRate << std::endl <<
				L"Total Elapsed Time: " << time.total.GetSeconds<float>();

			SetText(woss.str());
		}

	} // namespace components
} // namespace library
