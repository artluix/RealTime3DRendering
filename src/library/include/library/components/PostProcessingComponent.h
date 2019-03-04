#pragma once
#include "library/Components/DrawableComponent.h"
#include "library/Materials/PostProcessingMaterial.h"

#include "library/DirectXForwardDeclarations.h"

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

	protected:
		explicit PostProcessingComponent() = default;
		
		void InitializeQuad(const Application& app);
		void InitializeQuad(
			const Application& app,
			const std::string& techniqueName,
			const std::string& passName = "p0"
		);

	protected:
		ID3D11ShaderResourceView* m_sceneTexture = nullptr;
		std::unique_ptr<FullScreenQuadComponent> m_fullScreenQuad;
	};
} // namespace library
