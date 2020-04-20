#include "StdAfx.h"
#include "library/RenderTargets/DeferredLightingRenderTarget.h"

#include "library/Application.h"

namespace library
{
DeferredLightingRenderTarget::DeferredLightingRenderTarget(const Application& app)
{
	// setup render target buffers
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = app.GetScreenWidth();
	textureDesc.Height = app.GetScreenHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	auto device = app.GetDevice();

	m_rtBuffers.resize(3);
	m_rtBuffers[Color] = RenderTargetBuffer::Create(device, textureDesc);
	m_rtBuffers[Normal] = RenderTargetBuffer::Create(device, textureDesc);
	m_rtBuffers[WorldPosition] = RenderTargetBuffer::Create(device, textureDesc);

	m_vps.emplace_back(new Viewport(app.GetViewport()));
}

DeferredLightingRenderTarget::~DeferredLightingRenderTarget() = default;

} // namespace library
