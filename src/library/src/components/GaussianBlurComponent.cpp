#include "StdAfx.h"
#include "library/components/GaussianBlurComponent.h"

#include "library/Application.h"

#include "library/effect/Effect.h"
#include "library/effect/EffectVariable.h"

#include "library/Utils.h"
#include "library/Path.h"
#include "library/Color.h"
#include "library/Application.h"
#include "library/FullScreenRenderTarget.h"
#include "library/components/FullScreenQuadComponent.h"

#include <cmath>

namespace library
{
	namespace
	{
		constexpr float k_defaultBlurAmount = 1.f;

		const auto k_backgroundColor = library::Color::Black;

		const auto k_effectPath = utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
			Path("../data/effects/GaussianBlur_d.fxc")
#else
			Path("../data/effects/GaussianBlur.fxc")
#endif
		);
	}

	//-------------------------------------------------------------------------

	GaussianBlurComponent::GaussianBlurComponent(const Application& app)
		: ConcreteMaterialComponent(app)
		, m_blurAmount(k_defaultBlurAmount)
	{
	}

	GaussianBlurComponent::~GaussianBlurComponent() = default;

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

	void GaussianBlurComponent::Initialize()
	{
		m_effect = Effect::Create(m_app, k_effectPath);
		m_effect->LoadCompiled();

		m_material = std::make_unique<GaussianBlurMaterial>(*m_effect);
		m_material->Initialize();

		m_fullScreenQuad = std::make_unique<FullScreenQuadComponent>(m_app);
		m_fullScreenQuad->SetMaterial(*m_material, "blur", "p0");
		m_fullScreenQuad->Initialize();

		m_horizontalBlurTarget = std::make_unique<FullScreenRenderTarget>(m_app);
		m_verticalBlurTarget = std::make_unique<FullScreenRenderTarget>(m_app);

		InitializeSampleOffsets();
		InitializeSampleWeights();
	}

	//-------------------------------------------------------------------------

	void GaussianBlurComponent::InitializeSampleOffsets()
	{
		const float horizontalPixelSize = 1.f / m_app.GetScreenWidth();
		const float verticalPixelSize = 1.f / m_app.GetScreenHeight();

		auto& offsets = m_sample.offsets;

		const unsigned sampleOffsetsCount = m_material->GetSampleOffsets().GetTypeDesc().Elements;
		offsets.vertical.resize(sampleOffsetsCount);
		offsets.horizontal.resize(sampleOffsetsCount);

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
		weights.resize(sampleWeightsCount);
		
		float totalWeight = weights.front() = GetWeight(0.f);

		for (unsigned i = 0; i < (sampleWeightsCount >> 1); i++)
		{
			const float weight = GetWeight(i + 1.f);

			weights[(i << 1) + 1] = weight;
			weights[(i << 1) + 2] = -weight;

			totalWeight += weight * 2;
		}

		// Normalize weights
		auto normalizePred = [totalWeight](float& weight) { weight /= totalWeight; };
		std::for_each(weights.begin(), weights.end(), normalizePred);
	}

	//-------------------------------------------------------------------------

	void GaussianBlurComponent::Draw(const Time& time)
	{
		auto deviceContext = m_app.GetDeviceContext();

		// Horizontal Blur
		m_horizontalBlurTarget->Begin();
		deviceContext->ClearRenderTargetView(m_horizontalBlurTarget->GetRenderTargetView(), static_cast<const float*>(k_backgroundColor));
		deviceContext->ClearDepthStencilView(m_horizontalBlurTarget->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		m_fullScreenQuad->SetMaterialUpdateFunction(std::bind(&GaussianBlurComponent::UpdateHorizontalOffsets, this));
		m_fullScreenQuad->Draw(time);
		m_horizontalBlurTarget->End();

		// Vertical Blur for the final image
		m_fullScreenQuad->SetMaterialUpdateFunction(std::bind(&GaussianBlurComponent::UpdateVerticalOffsets, this));
		m_fullScreenQuad->Draw(time);
	}

	void GaussianBlurComponent::DrawToTexture(const Time& time)
	{

	}

	void GaussianBlurComponent::SetSceneTexture(ID3D11ShaderResourceView& sceneTexture)
	{
		if (m_sceneTexture != &sceneTexture)
		{
			m_sceneTexture = &sceneTexture;
		}
	}

	//-------------------------------------------------------------------------

	void GaussianBlurComponent::UpdateHorizontalOffsets()
	{
		m_material->GetColorTexture() << m_sceneTexture;
		m_material->GetSampleWeights() << m_sample.weights;
		m_material->GetSampleOffsets() << m_sample.offsets.horizontal;
	}

	void GaussianBlurComponent::UpdateVerticalOffsets()
	{
		m_material->GetColorTexture() << m_horizontalBlurTarget->GetOutputTexture();
		m_material->GetSampleWeights() << m_sample.weights;
		m_material->GetSampleOffsets() << m_sample.offsets.vertical;
	}

	//-------------------------------------------------------------------------

	float GaussianBlurComponent::GetWeight(const float x)
	{
		return exp(-(x * x) / (2 * m_blurAmount * m_blurAmount));
	}
} // namespace library
