#pragma once
#include "library/CommonTypes.h"
#include "library/NonCopyable.hpp"
#include "library/DirectXForwardDeclarations.h"

#include <vector>

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

	class DrawableComponent;
	class Application;
	struct Time;

	class Renderer : public NonCopyable<Renderer>
	{
	public:
		using Drawable = DrawableComponent;

		explicit Renderer(const Application& app);

		void AddDrawable(Drawable* const drawable);
		void RemoveDrawable(Drawable* const drawable);

		void RenderScene(const Time& time);
		void RenderText(const Time& time);

		void ResetRenderState(const RenderState rs = RenderState::All);
		void SaveRenderState(const RenderState rs = RenderState::All);
		void RestoreRenderState(const RenderState rs = RenderState::All);

		ComPtr<ID3D11RasterizerState> GetRasterizerState() { return m_rasterizerState; }
		ComPtr<ID3D11BlendState> GetBlendState() { return m_blendState; }
		ComPtr<ID3D11DepthStencilState> GetDepthStencilState() { return m_depthStencilState; }

	private:
		std::vector<Drawable*> m_sceneDrawables;
		std::vector<Drawable*> m_textDrawables;

		const Application& m_app;

		ComPtr<ID3D11RasterizerState> m_rasterizerState;
		ComPtr<ID3D11BlendState> m_blendState;
		ComPtr<ID3D11DepthStencilState> m_depthStencilState;

		std::array<float, 4> m_blendFactor;
		unsigned m_sampleMask;
		unsigned m_stencilRef;
	};
} // namespace library