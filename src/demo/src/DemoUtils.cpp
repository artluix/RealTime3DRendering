#include "DemoUtils.h"

#include <library/Components/KeyboardComponent.h>

namespace utils
{
	bool UpdateValue(
		float& value, // out
		const float stepValue,
		const library::math::Interval& interval,
		const library::KeyboardComponent& keyboard,
		const library::KeyPair& keys
	)
	{
		bool updated = false;

		if (keyboard.IsKeyDown(keys.first) && value < interval.max)
		{
			value += stepValue;
			value = library::math::Min(value, interval.max);
			updated = true;
		}

		if (keyboard.IsKeyDown(keys.second) && value > interval.min)
		{
			value -= stepValue;
			value = library::math::Max(value, interval.min);
			updated = true;
		}

		return updated;
	}
} // namespace utils
