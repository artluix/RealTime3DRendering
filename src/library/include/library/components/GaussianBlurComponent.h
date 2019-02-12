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

		void Initialize(const Application& app) override;
		void Draw(const Time& time) override;

		float GetBlurAmount() const { return m_blurAmount; }
		void SetBlurAmount(const float blurAmount);

	private:
		void InitializeSampleOffsets();
		void InitializeSampleWeights();

		void UpdateHorizontalOffsets();
		void UpdateVerticalOffsets();

		float GetWeight(const float x);

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
