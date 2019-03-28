#pragma once
#include "library/CommonTypes.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	struct BufferData
	{
		ComPtr<ID3D11Buffer> buffer;
		std::size_t elementsCount = 0;
	};
} // namespace library
