#include "StdAfx.h"
#include "library/Render/RenderBuffers.h"

namespace library
{
//RenderBufferBase::RenderBufferBase(
//	ID3D11Device* const device,
//	const D3D11_TEXTURE2D_DESC& textureDesc,
//	const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc /*= nullptr*/)
//{
//	auto hr = device->CreateTexture2D(&textureDesc, nullptr, &m_texture);
//	assert("ID3D11::CreateTexture2D() failed." && SUCCEEDED(hr));
//
//	if (textureDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
//	{
//		hr = device->CreateShaderResourceView(m_texture.Get(), srvDesc, &m_srv);
//		assert("ID3D11::CreateShaderResourceView() failed." && SUCCEEDED(hr));
//	}
//}

void RenderBufferBase::CreateTexture(ID3D11Device* const device, const D3D11_TEXTURE2D_DESC& textureDesc)
{
	auto hr = device->CreateTexture2D(&textureDesc, nullptr, &m_texture);
	assert("ID3D11::CreateTexture2D() failed." && SUCCEEDED(hr));
}

void RenderBufferBase::CreateSRV(ID3D11Device* const device, const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc /*= nullptr*/)
{
	auto hr = device->CreateShaderResourceView(m_texture.Get(), srvDesc, &m_srv);
	assert("ID3D11::CreateShaderResourceView() failed." && SUCCEEDED(hr));
}

//-------------------------------------------------------------------------

void RenderTargetBuffer::CreateView(ID3D11Device* const device, const D3D11_RENDER_TARGET_VIEW_DESC* viewDesc /*= nullptr*/)
{
	auto hr = device->CreateRenderTargetView(m_texture.Get(), viewDesc, &m_view);
	assert("ID3D11::CreateRenderTargetView() failed." && SUCCEEDED(hr));
}

RenderTargetBufferPtr RenderTargetBuffer::Create(
	ID3D11Device* const device,
	const D3D11_TEXTURE2D_DESC& textureDesc,
	const D3D11_RENDER_TARGET_VIEW_DESC* viewDesc /*= nullptr*/,
	const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc /*= nullptr*/)
{
	assert(textureDesc.BindFlags & D3D11_BIND_RENDER_TARGET);

	auto rtBuffer = new RenderTargetBuffer();

	rtBuffer->CreateTexture(device, textureDesc);

	if (textureDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		rtBuffer->CreateSRV(device, srvDesc);

	rtBuffer->CreateView(device, viewDesc);

	return RenderTargetBufferPtr(rtBuffer);
}

RenderTargetBufferPtr RenderTargetBuffer::Create(
	ID3D11Device* const device,
	ComPtr<ID3D11Texture2D>&& texture,
	const D3D11_RENDER_TARGET_VIEW_DESC* viewDesc /*= nullptr*/,
	const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc /*= nullptr*/)
{
	assert(!!texture);

	D3D11_TEXTURE2D_DESC textureDesc;
	texture->GetDesc(&textureDesc);

	assert(textureDesc.BindFlags & D3D11_BIND_RENDER_TARGET);

	auto rtBuffer = new RenderTargetBuffer();

	rtBuffer->m_texture = std::move(texture);

	if (textureDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		rtBuffer->CreateSRV(device, srvDesc);

	rtBuffer->CreateView(device, viewDesc);

	return RenderTargetBufferPtr(rtBuffer);
}

void RenderTargetBuffer::Clear(ID3D11DeviceContext* const deviceContext, const math::Color& color)
{
	deviceContext->ClearRenderTargetView(m_view.Get(), static_cast<const float*>(color));
}

//-------------------------------------------------------------------------

void DepthStencilBuffer::CreateView(ID3D11Device* const device, const D3D11_DEPTH_STENCIL_VIEW_DESC* viewDesc /*= nullptr*/)
{
	auto hr = device->CreateDepthStencilView(m_texture.Get(), viewDesc, &m_view);
	assert("ID3D11::CreateDepthStencilView() failed." && SUCCEEDED(hr));
}

DepthStencilBufferPtr DepthStencilBuffer::Create(
	ID3D11Device* const device,
	const D3D11_TEXTURE2D_DESC& textureDesc,
	const D3D11_DEPTH_STENCIL_VIEW_DESC* viewDesc /*= nullptr*/,
	const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc /*= nullptr*/)
{
	assert(textureDesc.BindFlags & D3D11_BIND_DEPTH_STENCIL);

	auto dsBuffer = new DepthStencilBuffer();

	dsBuffer->CreateTexture(device, textureDesc);

	if (textureDesc.BindFlags & D3D11_BIND_SHADER_RESOURCE)
		dsBuffer->CreateSRV(device, srvDesc);

	dsBuffer->CreateView(device, viewDesc);

	return DepthStencilBufferPtr(dsBuffer);
}

//-------------------------------------------------------------------------

void DepthStencilBuffer::Clear(
	ID3D11DeviceContext* const deviceContext,
	const UINT flags /*= (D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL)*/,
	const float depth /*= 1.f*/,
	const UINT stencil /*= 0 */)
{
	deviceContext->ClearDepthStencilView(m_view.Get(), flags, depth, stencil);
}

void DepthStencilBuffer::ClearDepth(ID3D11DeviceContext* const deviceContext, const float depth /*= 1.f*/)
{
	Clear(deviceContext, D3D11_CLEAR_DEPTH, depth);
}

void DepthStencilBuffer::ClearStencil(ID3D11DeviceContext* const deviceContext, const UINT stencil /*= 0*/)
{
	Clear(deviceContext, D3D11_CLEAR_STENCIL, 1.f, stencil);
}

} // namespace library
