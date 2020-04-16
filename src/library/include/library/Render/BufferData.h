#pragma once
#include "library/Common.h"
#include "library/DxForward.h"
#include "library/Render/Vertex.h"

namespace library
{
struct BufferData
{
	ComPtr<ID3D11Buffer> buffer;
	unsigned elementsCount = 0;

	BufferData() = default;

	explicit operator bool() const { return !!buffer; }

protected:
	explicit BufferData(const unsigned elementsCount) : elementsCount(elementsCount) {}

	void CreateBuffer(ID3D11Device* const device,
		const UINT bindFlags,
		const void* data, const std::size_t size
	);
};

//-------------------------------------------------------------------------

struct IndexBufferData : BufferData
{
	IndexBufferData() = default;

	IndexBufferData(IndexBufferData&&) = default;
	IndexBufferData& operator=(IndexBufferData&&) = default;

	template<typename T>
	IndexBufferData(ID3D11Device* const device, const ArrayBuffer<T>& indicesBuffer)
		: BufferData(unsigned(indicesBuffer.size))
	{
		static_assert(std::is_integral_v<T>, "IndexBuffer type must be integral");
		CreateBuffer(device, D3D11_BIND_INDEX_BUFFER, indicesBuffer.data, sizeof(T) * elementsCount);
	}
};

//-------------------------------------------------------------------------

struct VertexBufferData : BufferData
{
	unsigned stride = 0;
	unsigned offset = 0;

	VertexBufferData() = default;

	VertexBufferData(VertexBufferData&&) = default;
	VertexBufferData& operator=(VertexBufferData&&) = default;

	template<int ... VertexElementTypes>
	VertexBufferData(ID3D11Device* const device, const ArrayBuffer<Vertex<VertexElementTypes...>>& verticesBuffer)
		: BufferData(unsigned(verticesBuffer.size))
		, stride(sizeof(Vertex<VertexElementTypes...>))
	{
		CreateBuffer(device, D3D11_BIND_VERTEX_BUFFER, verticesBuffer.data, stride * elementsCount);
	}
};
} // namespace library
