#pragma once

namespace library
{
    template<class T>
    class NonConstructible
    {
        explicit NonConstructible() = delete;
        ~NonConstructible() = delete;

        explicit NonConstructible(NonConstructible&&) = delete;
        explicit NonConstructible(const NonConstructible&) = delete;
    };
}
