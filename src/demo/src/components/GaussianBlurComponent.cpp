#include "components/GaussianBlurComponent.h"

#include <library/components/KeyboardComponent.h>
#include <library/components/TextComponent.h>

#include <library/effect/Effect.h>
#include <library/effect/EffectVariable.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>
#include <library/Color.h>
#include <library/Application.h>
#include <library/FullScreenRenderTarget.h>
#include <library/components/FullScreenQuadComponent.h>

#include <cmath>
#include <sstream>
#include <iomanip>

namespace demo
{
	using namespace library;

	namespace
	{
		constexpr float k_defaultBlurAmount = 1.f;
		constexpr float k_blurModulationRate = 1.0f;

		const auto k_backgroundColor = library::Color::Black;
	}

	//-------------------------------------------------------------------------

	GaussianBlurComponent::GaussianBlurComponent()
		: m_blurAmount(k_defaultBlurAmount)
	{
	}

	GaussianBlurComponent::~GaussianBlurComponent() = default;

	//-------------------------------------------------------------------------

	void GaussianBlurComponent::Initialize(const Application& app)
	{
		InitializeMaterial(app, "GaussianBlur");
		InitializeQuad(app, "blur");

		m_text = std::make_unique<TextComponent>();
		m_text->SetPosition(math::Vector2(0.f, 45.f));
		m_text->SetTextGeneratorFunction(
			[this]() -> std::wstring
			{
				std::wostringstream woss;
				woss << std::setprecision(2) << "Blur Amount (+J/-K): " << m_blurAmount;

				return woss.str();
			}
		);
		m_text->Initialize(app);

		m_horizontalBlurTarget = std::make_unique<FullScreenRenderTarget>(app);
		m_verticalBlurTarget = std::make_unique<FullScreenRenderTarget>(app);

		InitializeSampleOffsets();
		InitializeSampleWeights();
	}

	//-------------------------------------------------------------------------

	void GaussianBlurComponent::Update(const library::Time& time)
	{
		UpdateBlurAmount(time);

		m_text->Update(time);
	}

	void GaussianBlurComponent::UpdateBlurAmount(const library::Time& time)
	{
		if (!!m_keyboard)
		{
			using namespace library;

			const auto elapsedTime = time.elapsed.GetSeconds();

			if (m_keyboard->IsKeyDown(Key::J))
			{
				m_blurAmount += k_blurModulationRate * elapsedTime;
				InitializeSampleWeights();
			}

			if (m_keyboard->IsKeyDown(Key::K) && m_blurAmount > 0.f)
			{
				m_blurAmount -= k_blurModulationRate * elapsedTime;
				m_blurAmount = math::Max(1e-10f, m_blurAmount);
				
				InitializeSampleWeights();
			}
		}
	}

	//-------------------------------------------------------------------------

	void GaussianBlurComponent::InitializeSampleOffsets()
	{
		const float horizontalPixelSize = 1.f / m_app->GetScreenWidth();
		const float verticalPixelSize = 1.f / m_app->GetScreenHeight();

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
			weights[(i << 1) + 2] = weight;

			totalWeight += weight * 2;
		}

		// Normalize weights
		auto normalizePred = [totalWeight](float& weight) { weight /= totalWeight; };
		std::for_each(weights.begin(), weights.end(), normalizePred);
	}

	//-------------------------------------------------------------------------

	void GaussianBlurComponent::Draw(const Time& time)
	{
		auto deviceContext = m_app->GetDeviceContext();

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

	//-------------------------------------------------------------------------

	void GaussianBlurComponent::UpdateHorizontalOffsets()
	{
		m_material->GetColorTexture() << GetSceneTexture();
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
} // namespace demo
