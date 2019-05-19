#include "StdAfx.h"
#include "library/Renderer.h"

#include "library/Components/UIComponent.h"
#include "library/Components/PrimitiveComponent.h"

#include "library/Effect/Effect.h"
#include "library/Materials/Material.h"

#include "library/Application.h"
#include "library/Time.h"

#include <algorithm>
#include <cassert>

namespace library
{

namespace
{
constexpr unsigned k_defaultSampleMask = -1;
constexpr unsigned k_defaultStencilRef = -1;

template <class Drawable>
class Finder
{
public:
	explicit Finder(Drawable& drawable) : m_drawable(drawable) {}

	bool operator()(Drawable& drawable) const { return &m_drawable == &drawable; }

private:
	Drawable& m_drawable;
};
} // namespace

//-------------------------------------------------------------------------

Renderer::Renderer(const Application& app)
	: m_app(app)
	, m_sampleMask(k_defaultSampleMask)
	, m_stencilRef(k_defaultStencilRef)
{
	m_blendFactor.fill(0.f);

	m_primitiveDrawables.reserve(20); // reserve some memory for drawables
	m_uiDrawables.reserve(20);		  // reserve some memory for drawables
}

//-------------------------------------------------------------------------

void Renderer::AddDrawable(PrimitiveComponent& primitiveDrawable)
{
	if (!primitiveDrawable.IsVisible())
		return;

	auto it =
		std::find_if(m_primitiveDrawables.begin(), m_primitiveDrawables.end(), Finder(primitiveDrawable));
	if (it != m_primitiveDrawables.end())
		return;

	m_primitiveDrawables.push_back(primitiveDrawable);
}

void Renderer::RemoveDrawable(PrimitiveComponent& primitiveDrawable)
{
	auto it =
		std::find_if(m_primitiveDrawables.begin(), m_primitiveDrawables.end(), Finder(primitiveDrawable));
	if (it == m_primitiveDrawables.end())
		return;

	m_primitiveDrawables.erase(it);
}

void Renderer::RenderScene(const Time& time)
{
	auto effectComp = [](const PrimitiveComponent& lhs, const PrimitiveComponent& rhs) {
		if (const auto lhsMat = lhs.GetMaterial())
			if (const auto rhsMat = rhs.GetMaterial())
				return lhsMat->GetEffect().GetName() < rhsMat->GetEffect().GetName();

		return false;
	};

	auto drawPred = [&time](PrimitiveComponent& primitiveDrawable) {
		primitiveDrawable.Draw(time);
	};

	auto drawables = m_primitiveDrawables;
	m_primitiveDrawables.clear();

	std::sort(drawables.begin(), drawables.end(), effectComp);
	std::for_each(drawables.begin(), drawables.end(), drawPred);
}

//-------------------------------------------------------------------------

void Renderer::AddDrawable(UIComponent& uiDrawable)
{
	if (!uiDrawable.IsVisible())
		return;

	auto it = std::find_if(m_uiDrawables.begin(), m_uiDrawables.end(), Finder(uiDrawable));
	if (it != m_uiDrawables.end())
		return;

	m_uiDrawables.push_back(uiDrawable);
}

void Renderer::RemoveDrawable(UIComponent& uiDrawable)
{
	auto it = std::find_if(m_uiDrawables.begin(), m_uiDrawables.end(), Finder(uiDrawable));
	if (it == m_uiDrawables.end())
		return;

	m_uiDrawables.erase(it);
}

void Renderer::RenderUI(const Time& time)
{
	auto drawPred = [&time](UIComponent& uiDrawable) {
		uiDrawable.Draw(time);
	};

	auto drawables = m_uiDrawables;
	m_uiDrawables.clear();

	SaveRenderState(RenderState::All);
	std::for_each(drawables.begin(), drawables.end(), drawPred);
	RestoreRenderState(RenderState::All);
}

//-------------------------------------------------------------------------

void Renderer::ResetRenderState(const RenderState rs /*= RenderState::All*/)
{
	auto deviceContext = m_app.GetDeviceContext();
	assert(deviceContext);

	switch (rs)
	{
		case RenderState::Rasterizer:
			deviceContext->RSSetState(nullptr);
			m_rasterizerState.Reset();
			break;

		case RenderState::Blend:
			deviceContext->OMSetBlendState(nullptr, nullptr, k_defaultSampleMask);
			m_blendState.Reset();
			break;

		case RenderState::DepthStencil:
			deviceContext->OMSetDepthStencilState(nullptr, k_defaultStencilRef);
			m_depthStencilState.Reset();
			break;

		case RenderState::All:
			ResetRenderState(RenderState::Rasterizer);
			ResetRenderState(RenderState::Blend);
			ResetRenderState(RenderState::DepthStencil);
			break;

		default: break;
	}
}

void Renderer::SaveRenderState(const RenderState rs /*= RenderState::All*/)
{
	auto deviceContext = m_app.GetDeviceContext();
	assert(deviceContext);

	switch (rs)
	{
		case RenderState::Rasterizer:
			m_rasterizerState.Reset();
			deviceContext->RSGetState(&m_rasterizerState);
			break;

		case RenderState::Blend:
			m_blendState.Reset();
			deviceContext->OMGetBlendState(&m_blendState, m_blendFactor.data(), &m_sampleMask);
			break;

		case RenderState::DepthStencil:
			m_depthStencilState.Reset();
			deviceContext->OMGetDepthStencilState(&m_depthStencilState, &m_stencilRef);
			break;

		case RenderState::All:
			SaveRenderState(RenderState::Rasterizer);
			SaveRenderState(RenderState::Blend);
			SaveRenderState(RenderState::DepthStencil);
			break;

		default: break;
	}
}

void Renderer::RestoreRenderState(const RenderState rs /*= RenderState::All*/)
{
	auto deviceContext = m_app.GetDeviceContext();
	assert(deviceContext);

	switch (rs)
	{
		case RenderState::Rasterizer: deviceContext->RSSetState(m_rasterizerState.Get()); break;

		case RenderState::Blend:
			deviceContext->OMSetBlendState(m_blendState.Get(), m_blendFactor.data(), m_sampleMask);
			break;

		case RenderState::DepthStencil:
			deviceContext->OMSetDepthStencilState(m_depthStencilState.Get(), m_stencilRef);
			break;

		case RenderState::All:
			RestoreRenderState(RenderState::Rasterizer);
			RestoreRenderState(RenderState::Blend);
			RestoreRenderState(RenderState::DepthStencil);
			break;

		default: break;
	}
}
} // namespace library
