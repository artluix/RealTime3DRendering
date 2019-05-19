#pragma once

namespace library
{
template <class T>
class NonCopyable
{
public:
	NonCopyable() = default;
	virtual ~NonCopyable() = default;

	// enable moving
	explicit NonCopyable(NonCopyable&&) = default;
	NonCopyable& operator=(NonCopyable&&) = default;

	// disable copying
	NonCopyable(const NonCopyable&) = delete;
	NonCopyable& operator=(const NonCopyable&) = delete;
};
} // namespace library
