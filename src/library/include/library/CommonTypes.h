#pragma once
#include "library/ComPtr.hpp"
#include "library/Logger.h"

#include <array>
#include <cstdint>
#include <type_traits>
#include <utility>
#include <vector>

namespace std
{
// clang-format off
template <class From, class To, class = void>
struct is_explicit_convertible : false_type {};

template <class From, class To>
struct is_explicit_convertible<
	From, To, decltype(declval<To&>() = static_cast<To>(declval<From&>()), void())> : true_type {};
// clang-format on

template <class From, class To>
inline constexpr bool is_explicit_convertible_v = is_explicit_convertible<From, To>::value;
} // namespace std

//-------------------------------------------------------------------------

namespace library
{
template <typename T>
inline constexpr bool is_std_vector = false;

template <typename T, typename A>
inline constexpr bool is_std_vector<std::vector<T, A>> = true;

template <typename T>
inline constexpr bool is_std_array = false;

template <typename T, std::size_t N>
inline constexpr bool is_std_array<std::array<T, N>> = true;

//-------------------------------------------------------------------------

using s8 = std::int8_t;
using s16 = std::int16_t;
using s32 = std::int32_t;
using s64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

//-------------------------------------------------------------------------

template <typename T>
using Pair = std::pair<T, T>;
} // namespace library
