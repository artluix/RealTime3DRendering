#pragma once

namespace library
{
    template<class T>
    class NonCopyable
    {
    public:
        explicit NonCopyable() = default;
        virtual ~NonCopyable() = default;
        
        // uncomment to enable moving
        //explicit NonCopyable(NonCopyable&&) = default;
        //NonCopyable& operator=(NonCopyable&&) = default;

        // disable copying
        explicit NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
    };
}
