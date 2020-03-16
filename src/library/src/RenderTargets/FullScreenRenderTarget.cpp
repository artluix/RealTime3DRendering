#include "StdAfx.h"
#include "library/RenderTargets/FullScreenRenderTarget.h"

#include "library/Application.h"

namespace library
{
FullScreenRenderTarget::FullScreenRenderTarget(const Application& app)
	: SingleRenderTarget(app)
{
	// setup render target buffer
	D3D11_TEXTURE2D_DESC textureDesc{};
	textureDesc.Width = app.GetScreenWidth();
	textureDesc.Height = app.GetScreenHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	//textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	auto device = app.GetDevice();
	m_rtBuffer = RenderTargetBuffer::Create(device, textureDesc);
}

FullScreenRenderTarget::~FullScreenRenderTarget() = default;
} // namespace library
