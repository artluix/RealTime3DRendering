#pragma once

// C++ 11 NonCopyable CRTP class


namespace library
{
    template<class T>
    class NonCopyable
    {
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;

        NonCopyable(NonCopyable&&) = delete;
        NonCopyable& operator=(NonCopyable&&) = delete;

    public:
        NonCopyable() = default;
        virtual ~NonCopyable() = default;
    };
}

