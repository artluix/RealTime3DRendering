#pragma once
#include "library/Common.h"
#include "library/Math/Color.h"

#include <d3d11_1.h>
#include <memory>

namespace library
{
class RenderBufferBase
{
public:
	ID3D11Texture2D* GetTexture() const { return m_texture.Get(); }
	ID3D11ShaderResourceView* GetSRV() const { return m_srv.Get(); }

protected:
	RenderBufferBase() = default;

	void CreateTexture(ID3D11Device* const device, const D3D11_TEXTURE2D_DESC& textureDesc);
	void CreateSRV(ID3D11Device* const device, const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr);

	ComPtr<ID3D11Texture2D> m_texture;
	ComPtr<ID3D11ShaderResourceView> m_srv;
};

//-------------------------------------------------------------------------

class RenderTargetBuffer;
using RenderTargetBufferPtr = std::unique_ptr<RenderTargetBuffer>;

class RenderTargetBuffer : public RenderBufferBase
{
public:
	static RenderTargetBufferPtr Create(
		ID3D11Device* const device,
		const D3D11_TEXTURE2D_DESC& textureDesc,
		const D3D11_RENDER_TARGET_VIEW_DESC* viewDesc = nullptr,
		const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr);

	static RenderTargetBufferPtr Create(
		ID3D11Device* const device,
		ComPtr<ID3D11Texture2D>&& texture,
		const D3D11_RENDER_TARGET_VIEW_DESC* viewDesc = nullptr,
		const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr);

	ID3D11RenderTargetView* GetView() const { return m_view.Get(); }
	ID3D11RenderTargetView*& GetView() { return m_view.Get(); }

	void Clear(ID3D11DeviceContext* const deviceContext, const math::Color& color);

protected:
	void CreateView(ID3D11Device* const device, const D3D11_RENDER_TARGET_VIEW_DESC* viewDesc = nullptr);

private:
	using RenderBufferBase::RenderBufferBase;

	ComPtr<ID3D11RenderTargetView> m_view;
};

//-------------------------------------------------------------------------

class DepthStencilBuffer;
using DepthStencilBufferPtr = std::unique_ptr<DepthStencilBuffer>;

class DepthStencilBuffer : public RenderBufferBase
{
public:
	static DepthStencilBufferPtr Create(
		ID3D11Device* const device,
		const D3D11_TEXTURE2D_DESC& textureDesc,
		const D3D11_DEPTH_STENCIL_VIEW_DESC* viewDesc = nullptr,
		const D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc = nullptr);

	ID3D11DepthStencilView* GetView() const { return m_view.Get(); }

	void Clear(
		ID3D11DeviceContext* const deviceContext,
		const UINT flags = (D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL),
		const float depth = 1.f,
		const UINT stencil = 0);
	void ClearDepth(ID3D11DeviceContext* const deviceContext, const float depth = 1.f);
	void ClearStencil(ID3D11DeviceContext* const deviceContext, const UINT stencil = 0);

protected:
	void CreateView(ID3D11Device* const device, const D3D11_DEPTH_STENCIL_VIEW_DESC* viewDesc = nullptr);

private:
	using RenderBufferBase::RenderBufferBase;

	ComPtr<ID3D11DepthStencilView> m_view;
};

} // namespace library
