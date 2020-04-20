#pragma once
#include <d3d11_1.h>
#include <memory>

namespace library
{
using Viewport = D3D11_VIEWPORT;
using ViewportPtr = std::unique_ptr<Viewport>;
} // namespace library
