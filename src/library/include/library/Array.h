#pragma once
#include <array>

namespace library
{
template<typename T, std::size_t Size>
using Array = std::array<T, Size>;

//-------------------------------------------------------------------------

template<typename T>
inline constexpr bool IsArray = false;

template<typename T, std::size_t Size>
inline constexpr bool IsArray<Array<T, Size>> = true;

//-------------------------------------------------------------------------

template<typename T, std::size_t Size>
constexpr auto MakeArray(const T& v)
{
	Array<T, Size> result{};
	for (auto& _v : result) _v = v;
	return result;
}

template<typename T, typename ... Ts>
constexpr auto MakeArray(T&& first, Ts&& ... args)
{
	return Array<T, 1 + sizeof...(Ts)>{ std::forward<T>(first), std::forward<Ts>(args)... };
}

/*template<typename T, std::size_t Size, typename ... Ts>
constexpr auto MakeArray(const Array<T, Size>& array, Ts&& ... args)
{
	return array + MakeArray(std::forward<Ts>(args)...));
}*/

template<typename T, std::size_t LeftSize, std::size_t RightSize>
constexpr auto operator+(const Array<T, LeftSize>& lhs, const Array<T, RightSize>& rhs)
{
	Array<T, LeftSize + RightSize> result{};
	std::size_t idx = 0;
	for (const auto& v : lhs) result[idx++] = v;
	for (const auto& v : rhs) result[idx++] = v;
	return result;
}

} // namespace library
