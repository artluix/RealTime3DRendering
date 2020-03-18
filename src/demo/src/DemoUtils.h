#pragma once
#include <library/Common.h>
#include <library/Math/Interval.h>
#include <library/Components/KeyboardComponent.h>

namespace library
{
using KeyPair = Pair<Key>;
} // namespace library

//-------------------------------------------------------------------------

namespace utils
{
template<typename T>
bool UpdateValue(
	T& value, // out
	const T stepValue,
	const library::math::IntervalType<T>& interval,
	const library::KeyboardComponent& keyboard,
	const library::KeyPair& keys)
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

template<typename T>
bool UpdateValueOnce(
	T& value, // out
	const T stepValue,
	const library::math::IntervalType<T>& interval,
	const library::KeyboardComponent& keyboard,
	const library::KeyPair& keys)
{
	bool updated = false;

	if (keyboard.WasKeyReleased(keys.first) && value < interval.max)
	{
		value += stepValue;
		value = library::math::Min(value, interval.max);
		updated = true;
	}

	if (keyboard.WasKeyReleased(keys.second) && value > interval.min)
	{
		value -= stepValue;
		value = library::math::Max(value, interval.min);
		updated = true;
	}

	return updated;
}
} // namespace utils
