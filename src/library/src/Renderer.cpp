#include "StdAfx.h"
#include "library/Renderer.h"

#include "library/components/DrawableComponent.h"
#include "library/components/TextComponent.h"
#include "library/components/SceneComponent.h"
#include "library/components/MaterialComponent.h"

#include "library/effect/Effect.h"
#include "library/materials/Material.h"

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
	}

	//-------------------------------------------------------------------------

	Renderer::Renderer(const Application& app)
		: m_app(app)
		, m_sampleMask(k_defaultSampleMask)
		, m_stencilRef(k_defaultStencilRef)
	{
		m_blendFactor.fill(0.f);

		m_drawables.reserve(50); // reserve some memory for drawables
	}

	//-------------------------------------------------------------------------

	void Renderer::AddDrawable(Drawable* const drawable)
	{
		auto it = std::find(m_drawables.begin(), m_drawables.end(), drawable);
		if (it != m_drawables.end())
			return;

		m_drawables.push_back(drawable);
	}

	void Renderer::RemoveDrawable(Drawable* const drawable)
	{
		auto it = std::find(m_drawables.begin(), m_drawables.end(), drawable);
		if (it == m_drawables.end())
			return;

		m_drawables.erase(it);
	}

	//-------------------------------------------------------------------------

	void Renderer::Render(const Time& time)
	{
		auto typeComp = [](const Drawable* lhs, const Drawable* rhs)
		{
			if (lhs->As<SceneComponent>() && rhs->As<TextComponent>())
				return true;

			return false;
		};

		auto effectComp = [](const Drawable* lhs, const Drawable* rhs)
		{
			if (const auto lhsMatComponent = lhs->As<MaterialComponent>())
				if (const auto rhsMatComponent = rhs->As<MaterialComponent>())
					return lhsMatComponent->GetMaterial().GetEffect().GetName() < rhsMatComponent->GetMaterial().GetEffect().GetName();

			return false;
		};

		auto textPred = [](const Drawable* drawable)
		{
			return !!drawable->As<TextComponent>();
		};

		auto drawPred = [&time](Drawable* drawable)
		{
			drawable->Draw(time);
		};

		//-------------------------------------------------------------------------

		// 1. Copy all drawables (prevent modification of vector)
		auto drawableNodes = m_drawables;
		m_drawables.clear();

		// 2. Separate 2D & 3D drawables
		std::sort(drawableNodes.begin(), drawableNodes.end(), typeComp);

		// 3. Find separate border
		auto uiIt = std::find_if(drawableNodes.begin(), drawableNodes.end(), textPred);

		// 4. Sort by effect and draw scene components
		if (uiIt != drawableNodes.begin()) // has scene components
		{
			std::sort(drawableNodes.begin(), uiIt, effectComp);
			std::for_each(drawableNodes.begin(), uiIt, drawPred);
		}

		// 5. Sort by effect and draw ui components
		if (uiIt != drawableNodes.end()) // has ui components
		{
			std::sort(uiIt, drawableNodes.end(), effectComp);
	
			SaveRenderState(RenderState::All);
			std::for_each(uiIt, drawableNodes.end(), drawPred);
			RestoreRenderState(RenderState::All);
		}
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

			default:
				break;
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
				deviceContext->RSGetState(m_rasterizerState.GetAddressOf());
				break;

			case RenderState::Blend:
				m_blendState.Reset();
				deviceContext->OMGetBlendState(m_blendState.GetAddressOf(), m_blendFactor.data(), &m_sampleMask);
				break;

			case RenderState::DepthStencil:
				m_depthStencilState.Reset();
				deviceContext->OMGetDepthStencilState(m_depthStencilState.GetAddressOf(), &m_stencilRef);
				break;

			case RenderState::All:
				SaveRenderState(RenderState::Rasterizer);
				SaveRenderState(RenderState::Blend);
				SaveRenderState(RenderState::DepthStencil);
				break;

			default:
				break;
		}
	}

	void Renderer::RestoreRenderState(const RenderState rs /*= RenderState::All*/)
	{
		auto deviceContext = m_app.GetDeviceContext();
		assert(deviceContext);

		switch (rs)
		{
			case RenderState::Rasterizer:
				deviceContext->RSSetState(m_rasterizerState.Get());
				break;

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

			default:
				break;
		}
	}
} // namespace library
