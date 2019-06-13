#pragma once
#include "library/Components/ConcreteMaterialPostProcessingComponent.hpp"
#include "library/Materials/GaussianBlurMaterial.h"
#include "library/Math/Vector.h"

namespace library
{
class GaussianBlurComponent : public ConcreteMaterialPostProcessingComponent<GaussianBlurMaterial>
{
	RTTI_CLASS(GaussianBlurComponent, PostProcessingComponent)

public:
	GaussianBlurComponent();
	~GaussianBlurComponent();

	float GetBlurAmount() const { return m_blurAmount; }
	void SetBlurAmount(const float blurAmount);

	void Draw(const Time& time) override;

	void DrawToTexture(const Time& time);
	ID3D11ShaderResourceView* GetOutputTexture() const { return m_outputTexture; }

protected:
	void InitializeInternal() override;

private:
	void InitializeSampleOffsets();
	void InitializeSampleWeights();

	void UpdateHorizontalOffsets(Material& material);
	void UpdateVerticalOffsets(Material& material);

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
