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
    enum class byte : std::uint8_t
    {
        //zero = 0
    };

    template <typename T>
    using Tuple2 = std::array<T, 2>;
} // namespace library
