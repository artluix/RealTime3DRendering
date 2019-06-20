#pragma once
#include "library/BufferData.h"

#include <d3dcommon.h>
#include <optional>

namespace library
{
struct PrimitiveData
{
	unsigned stride;							// mandatory
	VertexBufferData vertexBuffer;				// mandatory

	std::optional<IndexBufferData> indexBuffer; // optional
	D3D_PRIMITIVE_TOPOLOGY primitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//-------------------------------------------------------------------------

	PrimitiveData() = default;

	PrimitiveData(
		const unsigned stride,
		const D3D_PRIMITIVE_TOPOLOGY primitiveTopology,
		VertexBufferData&& vertexBuffer,
		IndexBufferData&& indexBuffer = {})
		: stride(stride)
		, vertexBuffer(std::move(vertexBuffer))
		, indexBuffer(std::move(indexBuffer))
		, primitiveTopology(primitiveTopology)
	{}

	PrimitiveData(PrimitiveData&&) = default;
	PrimitiveData& operator=(PrimitiveData&&) = default;
};
} // namespace library
