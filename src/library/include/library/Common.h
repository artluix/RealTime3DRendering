#pragma once
#include "library/Logger.h"

#include <array>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace library
{
	enum class byte : std::uint8_t {};

	template <typename T>
	using Tuple2 = std::array<T, 2>;

} // namespace library
