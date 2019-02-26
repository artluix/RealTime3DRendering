#include "StdAfx.h"
#include "library/Renderer.h"

#include "library/components/TextComponent.h"
#include "library/components/SceneComponent.h"

#include "library/effect/Effect.h"

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

		template<class Drawable>
		class Finder
		{
		public:
			explicit Finder(Drawable& drawable)
				: m_drawable(drawable)
			{
			}

			bool operator () (Drawable& drawable) const
			{
				return &m_drawable == &drawable;
			}

		private:
			Drawable& m_drawable;
		};
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

	void Renderer::AddDrawable(SceneComponent& sceneDrawable)
	{
		if (!sceneDrawable.IsVisible())
			return;

		auto it = std::find_if(m_sceneDrawables.begin(), m_sceneDrawables.end(), Finder(sceneDrawable));
		if (it != m_sceneDrawables.end())
			return;

		m_sceneDrawables.push_back(sceneDrawable);
	}

	void Renderer::RemoveDrawable(SceneComponent& sceneDrawable)
	{
		auto it = std::find_if(m_sceneDrawables.begin(), m_sceneDrawables.end(), Finder(sceneDrawable));
		if (it == m_sceneDrawables.end())
			return;

		m_sceneDrawables.erase(it);
	}

	void Renderer::RenderScene(const Time& time)
	{
		auto effectComp = [](const SceneComponent& lhs, const SceneComponent& rhs)
		{
			if (const auto lhsEffect = lhs.GetEffect())
				if (const auto rhsEffect = rhs.GetEffect())
					return lhsEffect->GetName() < rhsEffect->GetName();

			return false;
		};

		auto drawPred = [&time](SceneComponent& sceneDrawable)
		{
			sceneDrawable.Draw(time);
		};

		auto drawables = m_sceneDrawables;
		m_sceneDrawables.clear();

		std::sort(drawables.begin(), drawables.end(), effectComp);
		std::for_each(drawables.begin(), drawables.end(), drawPred);
	}

	//-------------------------------------------------------------------------

	void Renderer::AddDrawable(TextComponent& textDrawable)
	{
		if (!textDrawable.IsVisible())
			return;

		auto it = std::find_if(m_textDrawables.begin(), m_textDrawables.end(), Finder(textDrawable));
		if (it != m_textDrawables.end())
			return;

		m_textDrawables.push_back(textDrawable);
	}

	void Renderer::RemoveDrawable(TextComponent& textDrawable)
	{
		auto it = std::find_if(m_textDrawables.begin(), m_textDrawables.end(), Finder(textDrawable));
		if (it == m_textDrawables.end())
			return;

		m_textDrawables.erase(it);
	}

	void Renderer::RenderText(const Time& time)
	{
		auto drawPred = [&time](TextComponent& textDrawable)
		{
			textDrawable.Draw(time);
		};

		auto drawables = m_textDrawables;
		m_textDrawables.clear();

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
				deviceContext->OMGetBlendState(
					m_blendState.GetAddressOf(),
					m_blendFactor.data(),
					&m_sampleMask
				);
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
