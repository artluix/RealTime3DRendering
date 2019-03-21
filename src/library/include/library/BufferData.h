#pragma once
#include "library/CommonTypes.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	struct BufferData
	{
		ComPtr<ID3D11Buffer> buffer;
		unsigned elementsCount = 0;
	};
} // namespace library
