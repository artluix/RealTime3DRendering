#pragma once
#include <vector>

namespace library
{
template<typename T>
using DynArray = std::vector<T>;

//-------------------------------------------------------------------------

template<typename T>
inline constexpr bool IsDynArray = false;

template<typename T>
inline constexpr bool IsDynArray<DynArray<T>> = true;

} // namespace library
