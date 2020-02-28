#include "DemoUtils.h"

#include <library/Time.h>
#include <library/Components/KeyboardComponent.h>

namespace utils
{
	void UpdateValue(
		const library::Time& time,
		float& value, // out
		const library::math::Interval interval,
		const float modulationRate,
		const library::KeyboardComponent* keyboard,
		const library::Pair<library::Key> keys
	)
	{
		if (keyboard)
		{
			if (keyboard->IsKeyDown(keys.first) && value < interval.max)
			{
				value += modulationRate * time.elapsed.GetSeconds();
				value = library::math::Min(value, interval.max);
			}

			if (keyboard->IsKeyDown(keys.second) && value > interval.min)
			{
				value -= modulationRate * time.elapsed.GetSeconds();
				value = library::math::Max(value, interval.min);
			}
		}
	}
} // namespace utils
