#pragma once
#include "library/Components/ConcreteMaterialPostProcessingComponent.h"
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

protected:
	void InitializeInternal() override;

private:
	void InitializeSampleOffsets();
	void InitializeSampleWeights();

	void UpdateHorizontalOffsets(Material& material);
	void UpdateVerticalOffsets(Material& material);

	float GetWeight(const float x) const;

	std::unique_ptr<FullScreenRenderTarget> m_horizontalBlurTarget;
	//std::unique_ptr<FullScreenRenderTarget> m_verticalBlurTarget;

	float m_blurAmount;

	struct
	{
		struct
		{
			DynArray<math::Vector2> horizontal;
			DynArray<math::Vector2> vertical;
		} offsets;

		DynArray<float> weights;
	} m_sample;
};
} // namespace library
