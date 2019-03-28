#pragma once
#include "library/BufferData.h"

#include <optional>

namespace library
{
	struct MeshData
	{
		BufferData vertexBuffer; // mandatory
		ComPtr<ID3D11ShaderResourceView> texture; // mandatory
		std::optional<BufferData> indexBuffer; // optional
	};
} // namespace library
