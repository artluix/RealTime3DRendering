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
		virtual ~PostProcessingComponent();

		ID3D11ShaderResourceView* GetSceneTexture() const { return m_sceneTexture; }
		void SetSceneTexture(ID3D11ShaderResourceView& sceneTexture);

	protected:
		using MaterialComponent::MaterialComponent;
		
		void InitializeQuad(const std::string& techniqueName, const std::string& passName = "p0");

		std::unique_ptr<FullScreenQuadComponent> m_fullScreenQuad;

	private:
		ID3D11ShaderResourceView* m_sceneTexture = nullptr;
	};
} // namespace library
