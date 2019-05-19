#pragma once
#include "library/CommonTypes.h"

#include <DirectXMath.h>
#include <array>
#include <type_traits>
#include <vector>

namespace library::math
{
using XMVector = DirectX::XMVECTOR;
using XMMatrix = DirectX::XMMATRIX;

using XMVector2 = DirectX::XMFLOAT2;
using XMVector3 = DirectX::XMFLOAT3;
using XMVector4 = DirectX::XMFLOAT4;

using XMMatrix3 = DirectX::XMFLOAT3X3;
using XMMatrix4 = DirectX::XMFLOAT4X4;

//-------------------------------------------------------------------------
// helpers
//-------------------------------------------------------------------------

template <typename U, typename T, typename = std::enable_if_t<std::is_explicit_convertible_v<T, U>>>
std::vector<U> ToArray(const std::vector<T>& vecT)
{
	std::vector<U> vecU;
	vecU.reserve(vecT.size());

	for (const auto& v : vecT)
	{
		vecU.push_back(static_cast<U>(v));
	}

	return vecU;
}

template <
	typename U,
	typename T,
	std::size_t Count,
	typename = std::enable_if_t<std::is_explicit_convertible_v<T, U>>>
std::array<U, Count> ToArray(const std::array<T, Count>& arrT)
{
	std::array<XMVector, Count> arrU;

	for (std::size_t i = 0; i < Count; i++)
	{
		arrU[i] = static_cast<U>(arrT[i]);
	}

	return arrU;
}
} // namespace library::math
