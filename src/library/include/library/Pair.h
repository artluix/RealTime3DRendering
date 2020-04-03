#pragma once
#include "library/Array.h"

namespace library
{
template<typename T>
class Pair
{
public:
	using value_type = T;

	constexpr Pair() = default;
	constexpr Pair(T&& first, T&& last) : data{ first, last } {}
	constexpr Pair(const T& first, const T& last) : data{ first, last } {}

	const T& operator[](const unsigned idx) const { return data[idx]; }
	T& operator[](const unsigned idx) { return data[idx]; }

	/*const T& first() const { return data[0]; }
	T& first() { return data[0]; }

	const T& second() const { return data[1]; }
	T& second() { return data[1]; }*/

private:
	Array<T, 2> data;
};
} // namespace library
