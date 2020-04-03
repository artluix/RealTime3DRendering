#pragma once
#include <type_traits>

namespace std
{
template <class From, class To, class = void>
struct is_explicit_convertible : false_type {};

template <class From, class To>
struct is_explicit_convertible<
	From, To, decltype(declval<To&>() = static_cast<To>(declval<From&>()), void())> : true_type {};

template <class From, class To>
inline constexpr bool is_explicit_convertible_v = is_explicit_convertible<From, To>::value;

template <typename T>
using enum_type = underlying_type_t<T>;

template <typename T>
enable_if_t<is_enum_v<T>, enum_type<T>> from_enum(const T v)
{
	return static_cast<enum_type<T>>(v);
}
} // namespace std
