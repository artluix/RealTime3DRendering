#pragma once
#include <library/materials/GaussianBlurMaterial.h>

#include <library/components/PostProcessingComponentGlue.hpp>
#include <library/components/InputReceivableComponent.h>

#include <library/math/Vector.h>

namespace library
{
	class TextComponent;
} // namespace library

namespace demo
{
	class GaussianBlurComponent
		: public library::PostProcessingComponentGlue<library::GaussianBlurMaterial>
		, public library::InputReceivableComponent
	{
		RTTI_CLASS(GaussianBlurComponent, library::PostProcessingComponent, library::InputReceivableComponent)

	public:
		explicit GaussianBlurComponent(const library::Application& app);
		~GaussianBlurComponent();

		void Initialize() override;
		void Update(const library::Time& time) override;
		void Draw(const library::Time& time) override;

	private:
		void UpdateBlurAmount(const library::Time& time);

		void InitializeSampleOffsets();
		void InitializeSampleWeights();

		void UpdateHorizontalOffsets();
		void UpdateVerticalOffsets();

		float GetWeight(const float x);

		std::unique_ptr<library::FullScreenRenderTarget> m_horizontalBlurTarget;
		std::unique_ptr<library::FullScreenRenderTarget> m_verticalBlurTarget;

		std::unique_ptr<library::TextComponent> m_text;

		float m_blurAmount;

		struct
		{
			struct
			{
				std::vector<library::math::Vector2> horizontal;
				std::vector<library::math::Vector2> vertical;
			} offsets;

			std::vector<float> weights;
		} m_sample;
	};
} // namespace demo
