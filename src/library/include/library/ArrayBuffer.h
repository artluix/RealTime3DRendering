#pragma once
#include <library/Array.h>
#include <library/DynArray.h>

namespace library
{
template<typename T>
struct ArrayBuffer
{
	// Default
	constexpr ArrayBuffer(const T* data = nullptr, const std::size_t size = 0) : data(data), size(size) {}

	const T* data;
	std::size_t size;
};

//-------------------------------------------------------------------------

template<typename T, std::size_t Size>
constexpr auto MakeArrayBuffer(const Array<T, Size>& array)
{
	return ArrayBuffer(array.data(), Size);
}

template<typename T>
constexpr auto MakeArrayBuffer(const DynArray<T>& dynArray)
{
	return ArrayBuffer(dynArray.data(), dynArray.size());
}

} // namespace library
