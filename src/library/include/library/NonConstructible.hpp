#pragma once

namespace library
{
template <class T>
class NonConstructible
{
	NonConstructible() = delete;
	//~NonConstructible() = delete;

	NonConstructible(NonConstructible&&) = delete;
	NonConstructible(const NonConstructible&) = delete;
};
} // namespace library
