#pragma once
#include "library/components/MaterialDrawableComponent.h"
#include "library/components/FullScreenQuadComponent.h"
#include "library/materials/PostProcessingMaterial.h"

#include "library/DirectXForwardDeclarations.h"

#include <memory>

namespace library
{
	class FullScreenRenderTarget;
	class FullScreenQuadComponent;

	class MaterialPostProcessingComponent : public MaterialDrawableComponent
	{
		RTTI_CLASS(MaterialPostProcessingComponent, MaterialDrawableComponent)

	public:
		~MaterialPostProcessingComponent();

		ID3D11ShaderResourceView* GetSceneTexture() const { return m_sceneTexture; }
		void SetSceneTexture(ID3D11ShaderResourceView& sceneTexture);

		virtual const PostProcessingMaterial& GetMaterial() const = 0;

	protected:
		explicit MaterialPostProcessingComponent() = default;
		
		void InitializeQuad(const Application& app);
		void InitializeQuad(const Application& app, const std::string& techniqueName, const std::string& passName = "p0");

		virtual PostProcessingMaterial& GetMaterial() = 0;

		std::unique_ptr<FullScreenQuadComponent> m_fullScreenQuad;

	private:
		ID3D11ShaderResourceView* m_sceneTexture = nullptr;
	};
} // namespace library
