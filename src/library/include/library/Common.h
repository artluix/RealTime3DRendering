#pragma once

#include <windows.h>
#include <exception>
#include <cassert>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <memory>
#include <functional>

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <wrl/client.h>

// some types
namespace library
{
    enum class byte : unsigned char; // before c++17

    template <typename T>
    using Tuple2 = std::array<T, 2>;
}

using Microsoft::WRL::ComPtr;

using namespace DirectX;
using namespace DirectX::PackedVector;
