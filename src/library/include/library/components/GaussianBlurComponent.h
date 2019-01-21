#pragma once
#include "library/materials/GaussianBlurMaterial.h"
#include "library/components/MaterialComponent.h"
#include "library/math/Vector.h"

namespace library
{
	class FullScreenRenderTarget;
	class FullScreenQuadComponent;

	class GaussianBlurComponent : public ConcreteMaterialComponent<GaussianBlurMaterial>
	{
		RTTI_CLASS(GaussianBlurComponent, MaterialComponent)

	public:
		explicit GaussianBlurComponent(const Application& app);

		void Initialize() override;
		void Draw(const Time& time) override;
		void DrawToTexture(const Time& time);

		ID3D11ShaderResourceView* GetSceneTexture() const { return m_textureShaderResourceView.Get(); }
		void SetSceneTexture();

		ID3D11ShaderResourceView* GetOutputTexture() const { return m_outputTexture.Get(); }

		float GetBlurAmount() const { return m_blurAmount; }
		void SetBlurAmount(const float blurAmount);

	private:
		void InitializeSampleOffsets();
		void InitializeSampleWeights();

		void UpdateHorizontalOffsets();
		void UpdateVerticalOffsets();

		float GetWeight(const float x);

		ComPtr<ID3D11ShaderResourceView> m_outputTexture;
		std::unique_ptr<FullScreenQuadComponent> m_fullScreenQuad;
		std::unique_ptr<FullScreenRenderTarget> m_horizontalBlurTarget;
		std::unique_ptr<FullScreenRenderTarget> m_verticalBlurTarget;

		float m_blurAmount;

		struct
		{
			struct
			{
				std::vector<math::Vector2> horizontal;
				std::vector<math::Vector2> vertical;
			} offsets;

			std::vector<float> weights;
		} m_sample;
	};
} // namespace library
