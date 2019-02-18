#include "StdAfx.h"
#include "library/Renderer.h"

#include "library/components/DrawableComponent.h"
#include "library/components/TextComponent.h"
#include "library/components/SceneComponent.h"
#include "library/components/DrawableMaterialComponent.h"

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

		m_sceneDrawables.reserve(20); // reserve some memory for drawables
		m_textDrawables.reserve(20); // reserve some memory for drawables
	}

	//-------------------------------------------------------------------------

	void Renderer::AddDrawable(Drawable* const drawable)
	{
		if (!drawable->As<SceneComponent>() && !drawable->As<TextComponent>())
			return;

		auto& drawables = drawable->As<TextComponent>() ? m_textDrawables : m_sceneDrawables;

		auto it = std::find(drawables.begin(), drawables.end(), drawable);
		if (it != drawables.end())
			return;

		drawables.push_back(drawable);
	}

	void Renderer::RemoveDrawable(Drawable* const drawable)
	{
		if (!drawable->As<SceneComponent>() && !drawable->As<TextComponent>())
			return;

		auto& drawables = drawable->As<TextComponent>() ? m_textDrawables : m_sceneDrawables;

		auto it = std::find(drawables.begin(), drawables.end(), drawable);
		if (it == drawables.end())
			return;

		drawables.erase(it);
	}

	void Renderer::RenderScene(const Time& time)
	{
		auto effectComp = [](const Drawable* lhs, const Drawable* rhs)
		{
			if (const auto lhsMatComponent = lhs->As<DrawableMaterialComponent>())
				if (const auto rhsMatComponent = rhs->As<DrawableMaterialComponent>())
					return lhsMatComponent->GetMaterial().GetEffect().GetName() < rhsMatComponent->GetMaterial().GetEffect().GetName();

			return false;
		};

		auto drawPred = [&time](Drawable* drawable)
		{
			drawable->Draw(time);
		};

		auto drawables = m_sceneDrawables;
		m_sceneDrawables.clear();

		std::sort(drawables.begin(), drawables.end(), effectComp);
		std::for_each(drawables.begin(), drawables.end(),drawPred);
	}

	void Renderer::RenderText(const Time& time)
	{
		auto drawPred = [&time](Drawable* drawable)
		{
			drawable->Draw(time);
		};

		auto drawables = m_textDrawables;
		m_textDrawables.clear();

		SaveRenderState(RenderState::All);
		std::for_each(drawables.begin(), drawables.end(),drawPred);
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