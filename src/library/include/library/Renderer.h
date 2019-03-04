#pragma once
#include "library/CommonTypes.h"
#include "library/NonCopyable.hpp"
#include "library/DirectXForwardDeclarations.h"

#include <vector>
#include <functional>
#include <array>

namespace library
{
	enum class RenderState
	{
		Rasterizer,
		Blend,
		DepthStencil,

		//# All
		All
	};

	//-------------------------------------------------------------------------

	class Application;
	struct Time;

	class SceneComponent;
	class UIComponent;

	class Renderer : public NonCopyable<Renderer>
	{
	public:
		explicit Renderer(const Application& app);

		void AddDrawable(SceneComponent& sceneDrawable);
		void RemoveDrawable(SceneComponent& sceneDrawable);
		void RenderScene(const Time& time);

		void AddDrawable(UIComponent& uiDrawable);
		void RemoveDrawable(UIComponent& uiDrawable);
		void RenderUI(const Time& time);

		void ResetRenderState(const RenderState rs = RenderState::All);
		void SaveRenderState(const RenderState rs = RenderState::All);
		void RestoreRenderState(const RenderState rs = RenderState::All);

		ComPtr<ID3D11RasterizerState> GetRasterizerState() { return m_rasterizerState; }
		ComPtr<ID3D11BlendState> GetBlendState() { return m_blendState; }
		ComPtr<ID3D11DepthStencilState> GetDepthStencilState() { return m_depthStencilState; }

	private:
		std::vector<std::reference_wrapper<SceneComponent>> m_sceneDrawables;
		std::vector<std::reference_wrapper<UIComponent>> m_uiDrawables;

		const Application& m_app;

		ComPtr<ID3D11RasterizerState> m_rasterizerState;
		ComPtr<ID3D11BlendState> m_blendState;
		ComPtr<ID3D11DepthStencilState> m_depthStencilState;

		std::array<float, 4> m_blendFactor;
		unsigned m_sampleMask;
		unsigned m_stencilRef;
	};
} // namespace library
