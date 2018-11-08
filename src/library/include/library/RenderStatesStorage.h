#pragma once
#include "library/Common.h"
#include "library/NonCopyable.hpp"

struct ID3D11RasterizerState;
struct ID3D11BlendState;
struct ID3D11DepthStencilState;

namespace library
{
	enum class RenderState
	{
		Rasterizer,
		Blend,
		DepthStencil,

		// #All
		All
	};

	class Application;

	class RenderStatesStorage : public NonCopyable<RenderStatesStorage>
	{
	public:
		explicit RenderStatesStorage(const Application& app);
		~RenderStatesStorage() = default;

		void Reset(const RenderState rs = RenderState::All);

		void SaveState(const RenderState rs = RenderState::All);
		void RestoreState(const RenderState rs = RenderState::All);

		ID3D11RasterizerState* const GetRasterizerState() { return m_rasterizerState.Get(); }
		ID3D11BlendState* const GetBlendState() { return m_blendState.Get(); }
		ID3D11DepthStencilState* const GetDepthStencilState() { return m_depthStencilState.Get(); }

	private:
		const Application& m_app;

		ComPtr<ID3D11RasterizerState> m_rasterizerState;
		ComPtr<ID3D11BlendState> m_blendState;
		ComPtr<ID3D11DepthStencilState> m_depthStencilState;

		std::array<float, 4> m_blendFactor;
		unsigned m_sampleMask;
		unsigned m_stencilRef;
	};
} // namespace library
