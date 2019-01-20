#pragma once
#include "library/components/DrawableComponent.h"
#include "library/materials/GaussianBlurMaterial.h"

#include <memory>

namespace library
{
	class Effect;
	class FullScreenRenderTarget;
	class FullScreenQuadComponent;

	class GaussianBlurComponent : public DrawableComponent
	{
		RTTI_CLASS(GaussianBlurComponent, DrawableComponent)

	public:
		explicit GaussianBlurComponent(const Application& app);

		void Initialize() override;
		void Draw(const Time& time) override;

		ID3D11ShaderResourceView* GetOutputTexture() const;

		float GetBlurAmount() const { return m_blurAmount; }
		void SetBlurAmount(const float blurAmount);

	private:
		std::shared_ptr<Effect> m_effect;
		std::unique_ptr<GaussianBlurMaterial> m_material;

		std::unique_ptr<FullScreenQuadComponent> m_fullScreenQuad;
		std::unique_ptr<FullScreenRenderTarget> m_renderTarget;

		float m_blurAmount;
	};
} // namespace library
