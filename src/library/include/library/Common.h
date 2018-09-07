#pragma once
#include <array>

namespace library
{
    template <typename Enum>
    constexpr auto to_integral(Enum e) -> typename std::underlying_type<Enum>::type
    {
        return static_cast<typename std::underlying_type<Enum>::type>(e);
    }

    /////////////////////////////////////////////////////////////////

    // before c++17
    enum class byte : unsigned char
    {
        zero = 0
    };

    inline byte operator | (const byte lhs, const byte rhs)
    {
        return static_cast<byte>(to_integral(lhs) | to_integral(rhs));
    }

    inline byte operator & (const byte lhs, const byte rhs)
    {
        return static_cast<byte>(to_integral(lhs) & to_integral(rhs));
    }

    /////////////////////////////////////////////////////////////////

    template <typename T>
    using Tuple2 = std::array<T, 2>;
}
