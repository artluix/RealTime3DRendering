#pragma once
#include "library/Logger.h"

#include <cstdint>
#include <array>
#include <wrl/client.h>

namespace library
{
	using s8 = std::int8_t;
	using s16 = std::int16_t;
	using s32 = std::int32_t;
	using s64 = std::int64_t;
	
	using u8 = std::uint8_t;
	using u16 = std::uint16_t;
	using u32 = std::uint32_t;
	using u64 = std::uint64_t;

	enum class byte : std::uint8_t {};

	template <typename T>
	using Tuple2 = std::array<T, 2>;
} // namespace library

using Microsoft::WRL::ComPtr;
