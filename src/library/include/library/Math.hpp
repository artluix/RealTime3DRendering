#pragma once

namespace library
{
    namespace math
    {
        template <typename T>
        inline T Min(const T lhs, const T rhs)
        {
            return (lhs < rhs) ? lhs : rhs;
        }

        template <typename T>
        inline T Max(const T lhs, const T rhs)
        {
            return (lhs > rhs) ? lhs : rhs;
        }

        template <typename T>
        inline T Clamp(const T x, const T min, const T max)
        {
            return Min(Max(x, min), max);
        }
    }
}
