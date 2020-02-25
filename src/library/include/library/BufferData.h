#pragma once
#include "library/Common.h"
#include "library/VertexTypes.h"
#include "library/DxForward.h"
#include "library/InstanceData.h"

namespace library
{
struct BufferData
{
	ComPtr<ID3D11Buffer> buffer;
	unsigned elementsCount = 0;

	BufferData() = default;

protected:
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

	template<typename ContainerType>
	IndexBufferData(ID3D11Device* const device, const ContainerType& elements)
	{
		using ElementType = typename ContainerType::value_type;

		static_assert(
			is_std_vector<ContainerType> || is_std_array<ContainerType>,
			"BufferData can be created from std::array or std::vector only."
		);

		static_assert(std::is_integral_v<ElementType>, "IndexBuffer type must be integral");

		elementsCount = unsigned(elements.size());

		CreateBuffer(
			device,
			D3D11_BIND_INDEX_BUFFER,
			elements.data(),
			sizeof(ElementType) * elementsCount
		);
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

	template<typename ContainerType>
	VertexBufferData(ID3D11Device* const device, const ContainerType& elements)
	{
		using ElementType = typename ContainerType::value_type;

		static_assert(sizeof(ElementType) > 1, "Wrong stride!");
		stride = sizeof(ElementType);

		static_assert(
			is_std_vector<ContainerType> || is_std_array<ContainerType>,
			"BufferData can be created from std::array or std::vector only."
		);

		static_assert(
			std::is_base_of_v<Vertex, ElementType> ||
			std::is_same_v<InstanceData, ElementType>,
			"VertexBuffer type must be derived from Vertex"
		);

		elementsCount = static_cast<unsigned>(elements.size());

		CreateBuffer(
			device,
			D3D11_BIND_VERTEX_BUFFER,
			elements.data(),
			sizeof(ElementType) * elements.size()
		);
	}
};
} // namespace library
