#pragma once
#include "library/Common.h"
#include "library/NonConstructible.hpp"
#include "library/DirectXForwardDeclarations.h"

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

	class RenderStatesStorage : public NonConstructible<RenderStatesStorage>
	{
	public:
		static void SetDeviceContext(ID3D11DeviceContext* const deviceContext);
		static ID3D11DeviceContext* GetDeviceContext() { return s_deviceContext; }

		static void ResetState(const RenderState rs = RenderState::All);
		static void SaveState(const RenderState rs = RenderState::All);
		static void RestoreState(const RenderState rs = RenderState::All);

		static ID3D11RasterizerState* GetRasterizerState() { return s_rasterizerState.Get(); }
		static ID3D11BlendState* GetBlendState() { return s_blendState.Get(); }
		static ID3D11DepthStencilState* GetDepthStencilState() { return s_depthStencilState.Get(); }

		static void Reset();

	private:
		static ID3D11DeviceContext* s_deviceContext;

		static ComPtr<ID3D11RasterizerState> s_rasterizerState;
		static ComPtr<ID3D11BlendState> s_blendState;
		static ComPtr<ID3D11DepthStencilState> s_depthStencilState;

		static std::array<float, 4> s_blendFactor;
		static unsigned s_sampleMask;
		static unsigned s_stencilRef;
	};

} // namespace library
