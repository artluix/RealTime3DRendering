#pragma once
#include "library/Common.h"
#include "library/Path.h"
#include "library/Math/MathUtils.h"

#include <Unknwn.h>

namespace library::utils
{
std::wstring ToWideString(const std::string& str);

Path GetCurrentDirectory();
Path GetExecutableDirectory();

void WriteBinaryFile(const Path& path, const DynArray<std::byte>& data);
void WriteBinaryFile(const Path& path, const void* data, const std::size_t size);
void LoadBinaryFile(const Path& path, DynArray<std::byte>& data);

//-------------------------------------------------------------------------

template <typename U, typename T, typename = std::enable_if_t<std::is_explicit_convertible_v<T, U>>>
DynArray<U> ToArray(const DynArray<T>& vecT)
{
	DynArray<U> vecU;
	vecU.reserve(vecT.size());

	for (const auto& v : vecT)
	{
		vecU.push_back(static_cast<U>(v));
	}

	return vecU;
}

template <typename U, typename T, std::size_t Count, typename = std::enable_if_t<std::is_explicit_convertible_v<T, U>>>
Array<U, Count> ToArray(const Array<T, Count>& arrT)
{
	Array<U, Count> arrU;

	for (std::size_t i = 0; i < Count; i++)
	{
		arrU[i] = static_cast<U>(arrT[i]);
	}

	return arrU;
}

//-------------------------------------------------------------------------

constexpr unsigned GetMipLevelsCount(unsigned width, unsigned height)
{
	unsigned levelsCount = 1;
	while (width > 1 || height > 1)
	{
		width = math::Max(width >> 1, 1u);
		height = math::Max(height >> 1, 1u);
		levelsCount++;
	}

	return levelsCount;
}

} // namespace library::utils

//-------------------------------------------------------------------------

inline void Release(IUnknown* ptr)
{
	if (auto temp = ptr)
	{
		ptr = nullptr;
		temp->Release();
	}
}
