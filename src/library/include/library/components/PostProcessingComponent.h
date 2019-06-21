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

	ID3D11ShaderResourceView* GetSceneTexture() const { return m_sceneTexture; }
	void SetSceneTexture(ID3D11ShaderResourceView& sceneTexture);

	void Update(const Time& time) override;

protected:
	PostProcessingComponent() = default;

	void InitializeQuad();
	void InitializeQuad(const std::string& techniqueName, const std::string& passName = "p0");

protected:
	ID3D11ShaderResourceView* m_sceneTexture = nullptr;
	std::unique_ptr<FullScreenQuadComponent> m_fullScreenQuad;
};
} // namespace library
