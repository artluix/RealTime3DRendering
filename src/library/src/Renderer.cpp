#include "StdAfx.h"
#include "library/Renderer.h"

#include "library/components/DrawableComponent.h"
#include "library/components/UIComponent.h"
#include "library/components/SceneComponent.h"

#include "library/components/ProxyModelComponent.h"

#include "library/effect/Effect.h"
#include "library/effect/EffectMaterial.h"

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
	}

	//-------------------------------------------------------------------------

	void Renderer::RegisterForRender(Drawable* const drawable)
	{
		auto it = std::find(m_drawables.begin(), m_drawables.end(), drawable);
		if (it != m_drawables.end())
			return;

		m_drawables.push_back(drawable);
	}

	void Renderer::UnregisterForRender(Drawable* const drawable)
	{
		auto it = std::find(m_drawables.begin(), m_drawables.end(), drawable);
		if (it == m_drawables.end())
			return;

		m_drawables.erase(it);
	}

	//-------------------------------------------------------------------------

	void Renderer::Render(const Time& time)
	{
		auto effectComp = [](const Drawable* lhs, const Drawable* rhs)
		{
			const auto lhsMat = lhs->GetEffectMaterial();
			if (lhsMat)
				return false;

			const auto rhsMat = lhs->GetEffectMaterial();
			if (!rhsMat)
				return false;

			return lhsMat->GetEffect().GetName() < rhsMat->GetEffect().GetName();
		};

		auto typeComp = [](const Drawable* lhs, const Drawable* rhs)
		{
			if (lhs->As<SceneComponent>() && rhs->As<UIComponent>())
				return true;

			return false;
		};

		auto uiPred = [](const Drawable* drawable)
		{
			return !!drawable->As<UIComponent>();
		};

		auto drawPred = [&time](Drawable* drawable)
		{
			drawable->Draw(time);
		};

		//-------------------------------------------------------------------------

		// 1. copy all drawables (prevent modification of vector)
		auto drawables = m_drawables;
		
		// 2. Separate 2D & 3D drawables
		std::sort(drawables.begin(), drawables.end(), typeComp);

		// 3. Find separate border
		auto uiIt = std::find_if(drawables.begin(), drawables.end(), uiPred);

		// 4. Sort by effect and draw scene components
		if (uiIt != drawables.begin()) // has scene components
		{
			std::sort(drawables.begin(), uiIt, effectComp);
			std::for_each(drawables.begin(), uiIt, drawPred);
		}

		// 5. Sort by effect and draw ui components
		if (uiIt != drawables.end()) // has ui components
		{
			std::sort(uiIt, drawables.end(), effectComp);
	
			//SaveRenderState(RenderState::All);
			//std::for_each(uiIt, drawables.end(), drawPred);
			//RestoreRenderState(RenderState::All);
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
