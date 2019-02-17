#pragma once
#include "library/materials/GaussianBlurMaterial.h"
#include "library/components/ConcretePostProcessingComponent.hpp"

#include "library/math/Vector.h"

namespace library
{
	class GaussianBlurComponent : public ConcretePostProcessingComponent<GaussianBlurMaterial>
	{
		RTTI_CLASS(GaussianBlurComponent, PostProcessingComponent)

	public:
		explicit GaussianBlurComponent();
		~GaussianBlurComponent();

		float GetBlurAmount() const { return m_blurAmount; }
		void SetBlurAmount(const float blurAmount);

		void Initialize(const Application& app) override;
		void Draw(const Time& time) override;

		void DrawToTexture(const Time& time);
		ID3D11ShaderResourceView* GetOutputTexture() const { return m_outputTexture; }

	private:
		void InitializeSampleOffsets();
		void InitializeSampleWeights();

		void UpdateHorizontalOffsets();
		void UpdateVerticalOffsets();

		float GetWeight(const float x);

		std::unique_ptr<FullScreenRenderTarget> m_horizontalBlurTarget;
		std::unique_ptr<FullScreenRenderTarget> m_verticalBlurTarget;

		float m_blurAmount;

		ID3D11ShaderResourceView* m_outputTexture = nullptr;

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
