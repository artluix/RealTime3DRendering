#include "StdAfx.h"
#include "library/Render/BufferData.h"

namespace library
{
void BufferData::CreateBuffer(
	ID3D11Device* const device,
	const UINT bindFlags,
	const void* data, const std::size_t size
)
{
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.ByteWidth = static_cast<UINT>(size);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = bindFlags;

	D3D11_SUBRESOURCE_DATA subResourceData{};
	subResourceData.pSysMem = data;

	auto hr = device->CreateBuffer(
		&bufferDesc,
		&subResourceData,
		&buffer
	);
	assert("ID3D11Device::CreateBuffer() failed." && SUCCEEDED(hr));
}
} // namespace library
