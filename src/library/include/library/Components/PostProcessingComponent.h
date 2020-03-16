#pragma once
#include "library/Components/DrawableComponent.h"
#include "library/DxForward.h"

#include <memory>

namespace library
{
class FullScreenRenderTarget;
class FullScreenQuadComponent;

class PostProcessingComponent : public DrawableComponent
{
	RTTI_CLASS(PostProcessingComponent, DrawableComponent)

public:
	~PostProcessingComponent();

	ID3D11ShaderResourceView* GetSceneTextureSRV() const { return m_sceneTextureSRV; }
	void SetSceneTextureSRV(ID3D11ShaderResourceView* const sceneTextureSRV);

	void Update(const Time& time) override;

protected:
	PostProcessingComponent() = default;

	void InitializeQuad();
	void InitializeQuad(const std::string& techniqueName, const std::string& passName = "p0");

protected:
	ID3D11ShaderResourceView* m_sceneTextureSRV = nullptr;
	std::unique_ptr<FullScreenQuadComponent> m_fullScreenQuad;
};
} // namespace library
