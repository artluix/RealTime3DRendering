#include "StdAfx.h"
#include "library/RenderTargets/DeferredLightingRenderTarget.h"

#include "library/Math/Color.h"
#include "library/Application.h"

namespace library
{
DeferredLightingRenderTarget::DeferredLightingRenderTarget(const Application& app)
	:m_app(app)
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

	m_colorBuffer = RenderTargetBuffer::Create(device, textureDesc);
	m_normalBuffer = RenderTargetBuffer::Create(device, textureDesc);
	m_worldPositionBuffer = RenderTargetBuffer::Create(device, textureDesc);
}

DeferredLightingRenderTarget::~DeferredLightingRenderTarget() = default;

//-------------------------------------------------------------------------

void DeferredLightingRenderTarget::Begin()
{
	ViewData viewData;

	std::array<ID3D11RenderTargetView*, 3> rtvs = {
		m_colorBuffer->GetView(),
		m_normalBuffer->GetView(),
		m_worldPositionBuffer->GetView()
	};

	viewData.rtvsCount = unsigned(rtvs.size());
	viewData.rtvs = rtvs.data();

	viewData.vpsCount = 1;
	viewData.vps = &m_app.GetViewport();

	RenderTarget::Begin(viewData);
}

} // namespace library
