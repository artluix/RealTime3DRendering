#include "StdAfx.h"
#include "library/RenderTargets/MultipleRenderTarget.h"

#include "library/Math/Color.h"
#include "library/Application.h"

namespace library
{
MultipleRenderTarget::MultipleRenderTarget(const Application& app, const unsigned size)
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

	m_rtBuffers.reserve(size);
	for (unsigned i = 0; i < size; i++)
	{
		m_rtBuffers.emplace_back(std::move(RenderTargetBuffer::Create(device, textureDesc)));
	}
}

MultipleRenderTarget::~MultipleRenderTarget() = default;

//-------------------------------------------------------------------------

void MultipleRenderTarget::Begin()
{
	ViewData viewData;

	std::vector<ID3D11RenderTargetView*> rtvs;
	for (const auto& rtBuffer : m_rtBuffers)
	{
		rtvs.emplace_back(rtBuffer->GetView());
	}

	viewData.rtvsCount = unsigned(rtvs.size());
	viewData.rtvs = rtvs.data();

	viewData.vpsCount = 1;
	viewData.vps = &m_app.GetViewport();

	RenderTarget::Begin(viewData);
}

} // namespace library
