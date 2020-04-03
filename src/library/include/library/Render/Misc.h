#pragma once
#include "library/ArrayBuffer.h"

#include <d3d11.h>

namespace library
{
using InputElementDesc = D3D11_INPUT_ELEMENT_DESC;

using InputElementDescDynArray = DynArray<D3D11_INPUT_ELEMENT_DESC>;

template<std::size_t Size>
using InputElementDescArray = Array<D3D11_INPUT_ELEMENT_DESC, Size>;

using InputElementDescArrayBuffer = ArrayBuffer<InputElementDesc>;

} // namespace library
