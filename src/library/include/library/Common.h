#pragma once
#include "library/ComPtr.hpp"
#include "library/Logger.h"

#include <cstdint>
#include <type_traits>
#include <utility>
#include <array>
#include <vector>

namespace std
{
template <class From, class To, class = void>
struct is_explicit_convertible : false_type {};

template <class From, class To>
struct is_explicit_convertible<
	From, To, decltype(declval<To&>() = static_cast<To>(declval<From&>()), void())> : true_type {};

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

template <typename T>
using Pair = std::pair<T, T>;
} // namespace library
