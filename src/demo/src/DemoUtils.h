#pragma once
#include <library/Common.h>
#include <library/Math/Interval.h>

namespace library
{
	struct Time;
	enum class Key : std::uint8_t;
	class KeyboardComponent;
} // namespace library

//-------------------------------------------------------------------------

namespace utils
{
	void UpdateValue(
		const library::Time& time,
		float& value, // out
		const library::math::Interval& interval,
		const float modulationRate,
		const library::KeyboardComponent* keyboard,
		const library::Pair<library::Key>& keys // (increaseKey, decreaseKey)
	);
} // namespace utils
