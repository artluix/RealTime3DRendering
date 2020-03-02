#pragma once
#include <library/Common.h>
#include <library/Math/Interval.h>

namespace library
{
	enum class Key : std::uint8_t;
	using KeyPair = Pair<Key>;
	class KeyboardComponent;
} // namespace library

//-------------------------------------------------------------------------

namespace utils
{
	bool UpdateValue(
		float& value, // out
		const float stepValue,
		const library::math::Interval& interval,
		const library::KeyboardComponent& keyboard,
		const library::KeyPair& keys // (increaseKey, decreaseKey)
	);
} // namespace utils
