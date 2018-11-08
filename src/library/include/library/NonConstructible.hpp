#pragma once

namespace library
{
	template<class T>
	class NonConstructible
	{
		explicit NonConstructible() = delete;

		explicit NonConstructible(NonConstructible&&) = delete;
		explicit NonConstructible(const NonConstructible&) = delete;
	};
} // namespace library
