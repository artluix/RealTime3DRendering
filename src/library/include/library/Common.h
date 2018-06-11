#pragma once

#include <windows.h>
#include <exception>
#include <cassert>
#include <string>
#include <vector>
#include <map>
#include <memory>

#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <wrl/client.h>

// some types
namespace library
{
    enum class byte : unsigned char;
}

using Microsoft::WRL::ComPtr;

using namespace DirectX;
using namespace DirectX::PackedVector;
