#pragma once
#include "library/Common.h"
#include "library/NonCopyable.h"
#include "library/DxForward.h"

#include <functional>

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
class PrimitiveComponent;
class UIComponent;
class PostProcessingComponent;
class SingleRenderTarget;

class Renderer : public NonCopyable<Renderer>
{
public:
	explicit Renderer(const Application& app);

	void AddDrawable(PrimitiveComponent& primitiveDrawable);
	void RemoveDrawable(PrimitiveComponent& primitiveDrawable);

	void SetPostProcessing(PostProcessingComponent* pp);

	ID3D11ShaderResourceView* GetSceneTextureSRV() const;

	void AddDrawable(UIComponent& uiDrawable);
	void RemoveDrawable(UIComponent& uiDrawable);

	void RenderScene(const Time& time);
	void RenderUI(const Time& time);
	void Render(const Time& time);

	void UnbindPSResources(const unsigned startIdx, const unsigned count);

	void ResetRenderState(const RenderState rs = RenderState::All);
	void SaveRenderState(const RenderState rs = RenderState::All);
	void RestoreRenderState(const RenderState rs = RenderState::All);

	ID3D11RasterizerState* GetRasterizerState() { return m_rasterizerState.Get(); }
	ID3D11BlendState* GetBlendState() { return m_blendState.Get(); }
	ID3D11DepthStencilState* GetDepthStencilState() { return m_depthStencilState.Get(); }

private:
	DynArray<std::reference_wrapper<PrimitiveComponent>> m_primitiveDrawables;
	DynArray<std::reference_wrapper<UIComponent>> m_uiDrawables;

	std::unique_ptr<SingleRenderTarget> m_sceneRT;
	PostProcessingComponent* m_postProcessing = nullptr;

	const Application& m_app;

	ComPtr<ID3D11RasterizerState> m_rasterizerState;
	ComPtr<ID3D11BlendState> m_blendState;
	ComPtr<ID3D11DepthStencilState> m_depthStencilState;

	Array<float, 4> m_blendFactor;
	unsigned m_sampleMask;
	unsigned m_stencilRef;
};
} // namespace library
