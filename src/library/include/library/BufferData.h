#pragma once
#include "library/CommonTypes.h"
#include "library/VertexTypes.h"
#include "library/DirectXForwardDeclarations.h"
#include "library/Exception.h"

namespace library
{
	enum class BufferType
	{
		Vertex,
		Index
	};

	//-------------------------------------------------------------------------

	template<BufferType BT>
	struct BufferData
	{
		ComPtr<ID3D11Buffer> buffer;
		unsigned elementsCount = 0;

		BufferData() = default;

		template<typename ContainerType>
		BufferData(ID3D11Device* const device, const ContainerType& elements);

		BufferData(BufferData&&) = default;
		BufferData& operator = (BufferData&&) = default;

	private:
		void CreateBuffer(ID3D11Device* const device, const void* data, const std::size_t size);
	};

	//-------------------------------------------------------------------------

	template<BufferType BT>
	template<typename ContainerType>
	BufferData<BT>::BufferData(ID3D11Device* const device, const ContainerType& elements)
		: elementsCount(static_cast<unsigned>(elements.size()))
	{
		static_assert(
			is_std_vector<ContainerType> || is_std_array<ContainerType>,
			"BufferData can be created from std::array or std::vector only.");

		if constexpr (BT == BufferType::Vertex)
		{
			static_assert(
				std::is_base_of_v<Vertex, ContainerType::value_type>,
				"VertexBuffer type must be derived from Vertex");
		}
		else if constexpr (BT == BufferType::Index)
		{
			static_assert(
				std::is_integral_v<ContainerType::value_type>,
				"IndexBuffer type must be integral");
		}
		else
		{
			static_assert(false, "Wrong buffer type");
		}

		/*switch (BT)
		{
			case BufferType::Vertex:
				static_assert(
					std::is_base_of_v<Vertex, ContainerType::value_type>,
					"VertexBuffer type must be derived from Vertex");
				break;

			case BufferType::Index:
				static_assert(
					std::is_integral_v<ContainerType::value_type>,
					"IndexBuffer type must be integral");
				break;

			default:
				static_assert(false, "Wrong buffer type");
				break;
		};*/

		CreateBuffer(device, elements.data(), sizeof(typename ContainerType::value_type) * elements.size());
	}

	//-------------------------------------------------------------------------

	template<BufferType BT>
	void BufferData<BT>::CreateBuffer(ID3D11Device* const device, const void* data, const std::size_t size)
	{
		D3D11_BUFFER_DESC bufferDesc{};
		bufferDesc.ByteWidth = static_cast<UINT>(size);
		bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;

		switch (BT)
		{
			case BufferType::Vertex:
				bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				break;

			case BufferType::Index:
				bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
				break;

			default:
				break;
		};

		D3D11_SUBRESOURCE_DATA subResourceData{};
		subResourceData.pSysMem = data;

		auto hr = device->CreateBuffer(
			&bufferDesc,
			&subResourceData,
			&buffer
		);
		if (FAILED(hr))
		{
			throw Exception("ID3D11Device::CreateBuffer() failed.");
		}
	}

	//-------------------------------------------------------------------------

	template struct BufferData<BufferType::Vertex>;
	template struct BufferData<BufferType::Index>;

	using VertexBufferData = BufferData<BufferType::Vertex>;
	using IndexBufferData = BufferData<BufferType::Index>;
} // namespace library
