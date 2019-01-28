#pragma once
#include "library/components/MaterialComponent.h"
#include "library/DirectXForwardDeclarations.h"

namespace library
{
	class FullScreenRenderTarget;
	class FullScreenQuadComponent;

	class PostProcessingComponent : public MaterialComponent
	{
		RTTI_CLASS(PostProcessingComponent, MaterialComponent)

	public:
		~PostProcessingComponent();

		ID3D11ShaderResourceView* GetSceneTexture() const { return m_sceneTexture; }
		void SetSceneTexture(ID3D11ShaderResourceView& sceneTexture);

	protected:
		explicit PostProcessingComponent() = default;
		
		void InitializeQuad(const Application& app, const std::string& techniqueName, const std::string& passName = "p0");

		std::unique_ptr<FullScreenQuadComponent> m_fullScreenQuad;

	private:
		ID3D11ShaderResourceView* m_sceneTexture = nullptr;
	};
} // namespace library
