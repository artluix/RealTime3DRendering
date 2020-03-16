#pragma once
#include "library/Render/BufferData.h"

#include <d3dcommon.h>
#include <optional>

namespace library
{
struct PrimitiveData
{
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	VertexBufferData vertexBuffer;				// mandatory
	std::optional<IndexBufferData> indexBuffer; // optional

	//-------------------------------------------------------------------------

	PrimitiveData() = default;

	PrimitiveData(
		const D3D_PRIMITIVE_TOPOLOGY primitiveTopology,
		VertexBufferData&& vertexBuffer,
		IndexBufferData&& indexBuffer = {}
	)
		: vertexBuffer(std::move(vertexBuffer))
		, indexBuffer(std::move(indexBuffer))
		, primitiveTopology(primitiveTopology)
	{}

	PrimitiveData(PrimitiveData&&) = default;
	PrimitiveData& operator=(PrimitiveData&&) = default;
};
} // namespace library
