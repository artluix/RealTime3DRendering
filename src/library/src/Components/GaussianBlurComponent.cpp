#include "StdAfx.h"
#include "library/Components/GaussianBlurComponent.h"

#include "library/Components/FullScreenQuadComponent.h"

#include "library/Effect/Effect.h"
#include "library/Effect/EffectVariable.h"

#include "library/Application.h"
#include "library/Utils.h"
#include "library/Path.h"
#include "library/Math/Color.h"
#include "library/Application.h"

#include "library/RenderTargets/FullScreenRenderTarget.h"

namespace library
{

namespace
{
constexpr float k_defaultBlurAmount = 1.f;
constexpr float k_blurModulationRate = 1.0f;

constexpr auto k_backgroundColor = colors::Black;
} // namespace

//-------------------------------------------------------------------------

GaussianBlurComponent::GaussianBlurComponent()
	: m_blurAmount(k_defaultBlurAmount)
{}

GaussianBlurComponent::~GaussianBlurComponent() = default;

//-------------------------------------------------------------------------

void GaussianBlurComponent::InitializeInternal()
{
	CreateMaterialWithEffect("GaussianBlur");
	InitializeQuad("blur");

	m_horizontalBlurTarget = std::make_unique<FullScreenRenderTarget>(GetApp());

	InitializeSampleOffsets();
	InitializeSampleWeights();
}

//-------------------------------------------------------------------------

void GaussianBlurComponent::InitializeSampleOffsets()
{
	const float horizontalPixelSize = 1.f / GetApp().GetScreenWidth();
	const float verticalPixelSize = 1.f / GetApp().GetScreenHeight();

	auto& offsets = m_sample.offsets;

	const unsigned sampleOffsetsCount = m_material->GetSampleOffsets().GetTypeDesc().Elements;
	if (sampleOffsetsCount != unsigned(offsets.horizontal.size()))
	{
		offsets.horizontal.resize(sampleOffsetsCount);
		offsets.vertical.resize(sampleOffsetsCount);
	}

	offsets.vertical.front() = offsets.horizontal.front() = math::Vector2::Zero;

	for (unsigned i = 0; i < (sampleOffsetsCount >> 1); i++)
	{
		const float sampleOffset = (i << 1) + 1.5f;

		const math::Vector2 horizontalOffset(horizontalPixelSize * sampleOffset, 0.f);
		const math::Vector2 verticalOffset(0.f, verticalPixelSize * sampleOffset);

		offsets.horizontal[(i << 1) + 1] = horizontalOffset;
		offsets.horizontal[(i << 1) + 2] = -horizontalOffset;

		offsets.vertical[(i << 1) + 1] = verticalOffset;
		offsets.vertical[(i << 1) + 2] = -verticalOffset;
	}
}

void GaussianBlurComponent::InitializeSampleWeights()
{
	auto& weights = m_sample.weights;

	const unsigned sampleWeightsCount = m_material->GetSampleWeights().GetTypeDesc().Elements;
	if (sampleWeightsCount != unsigned(weights.size()))
		weights.resize(sampleWeightsCount);

	float totalWeight = weights.front() = GetWeight(0.f);

	for (unsigned i = 0; i < (sampleWeightsCount >> 1); i++)
	{
		const float weight = GetWeight(i + 1.f);

		weights[(i << 1) + 1] = weight;
		weights[(i << 1) + 2] = weight;

		totalWeight += weight * 2;
	}

	// Normalize weights
	auto normalizePred = [totalWeight](float& weight) {
		weight /= totalWeight;
	};
	std::for_each(weights.begin(), weights.end(), normalizePred);
}

//-------------------------------------------------------------------------

void GaussianBlurComponent::Draw(const Time& time)
{
	// Horizontal Blur
	m_horizontalBlurTarget->Begin();
	m_horizontalBlurTarget->Clear(k_backgroundColor);

	m_fullScreenQuad->SetMaterialUpdateFunction(std::bind(&GaussianBlurComponent::UpdateHorizontalOffsets, this, std::ref(*m_material)));
	m_fullScreenQuad->Draw(time);
	m_horizontalBlurTarget->End();

	// Vertical Blur for final image
	m_fullScreenQuad->SetMaterialUpdateFunction(std::bind(&GaussianBlurComponent::UpdateVerticalOffsets, this, std::ref(*m_material)));
	m_fullScreenQuad->Draw(time);
}

//-------------------------------------------------------------------------

void GaussianBlurComponent::SetBlurAmount(const float blurAmount)
{
	if (m_blurAmount != blurAmount)
	{
		m_blurAmount = blurAmount;
		InitializeSampleWeights();
	}
}

//-------------------------------------------------------------------------

void GaussianBlurComponent::UpdateHorizontalOffsets(Material& material)
{
	material.GetSceneTexture() << m_sceneTextureSRV;
	material.GetSampleWeights() << m_sample.weights;
	material.GetSampleOffsets() << m_sample.offsets.horizontal;
}

void GaussianBlurComponent::UpdateVerticalOffsets(Material& material)
{
	material.GetSceneTexture() << m_horizontalBlurTarget->GetRenderTargetBuffer()->GetSRV();
	material.GetSampleWeights() << m_sample.weights;
	material.GetSampleOffsets() << m_sample.offsets.vertical;
}

//-------------------------------------------------------------------------

float GaussianBlurComponent::GetWeight(const float x) const
{
	return exp(-(x * x) / (2 * m_blurAmount * m_blurAmount));
}
} // namespace library
