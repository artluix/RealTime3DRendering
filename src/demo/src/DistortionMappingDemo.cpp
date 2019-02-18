#include "DistortionMappingDemo.h"

#include <library/components/FullScreenQuadComponent.h>
#include <library/components/TextComponent.h>
#include <library/components/KeyboardComponent.h>

#include <library/Application.h>
#include <library/Time.h>
#include <library/FullScreenRenderTarget.h>

#include <library/effect/EffectVariable.h>

#include <library/math/Math.h>

#include <sstream>
#include <iomanip>

using namespace library;

namespace
{
	const auto k_backgroundColor = Color::Black;
}

DistortionMappingDemo::DistortionMappingDemo()
	: m_displacementScale(1.f)
{
}

DistortionMappingDemo::~DistortionMappingDemo() = default;

void DistortionMappingDemo::Initialize(const Application& app)
{
	PostProcessingComponent::Initialize(app);

	InitializeMaterial(app, "DistortionMapping");
	InitializeQuad(app, "distortion_map");
	m_fullScreenQuad->SetMaterialUpdateFunction(std::bind(&DistortionMappingDemo::UpdateDistortionMapMaterial, this));

	m_distortionMapTexture = app.LoadTexture("DistortionGlass");

	m_sceneRenderTarget = std::make_unique<FullScreenRenderTarget>(app);

	m_text = std::make_unique<TextComponent>();
	m_text->SetTextGeneratorFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss
				<< L"Displacement Scale (+Comma/-Period): " << m_displacementScale;
			return woss.str();
		}
	);
	m_text->SetPosition(math::Vector2(0.f, 200.f));
	m_text->Initialize(app);
}

void DistortionMappingDemo::Update(const Time& time)
{
	m_text->Update(time);

	UpdateDisplacementScale(time);

	DrawableComponent::Update(time);
}

void DistortionMappingDemo::Draw(const Time& time)
{
	/*auto deviceContext = m_app->GetDeviceContext();

	m_sceneRenderTarget->Begin();

	deviceContext->ClearRenderTargetView(m_sceneRenderTarget->GetRenderTargetView(), static_cast<const float*>(k_backgroundColor));
	deviceContext->ClearDepthStencilView(m_sceneRenderTarget->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_sceneRenderTarget->End();

	m_app->UnbindPixelShaderResources(0, 1);*/
	m_fullScreenQuad->Draw(time);
}

void DistortionMappingDemo::UpdateDisplacementScale(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		if (m_keyboard->IsKeyDown(Key::Comma) && m_displacementScale < 3.0f)
		{
			m_displacementScale += elapsedTime;
			m_displacementScale = math::Min(m_displacementScale, 3.0f);
		}

		if (m_keyboard->IsKeyDown(Key::Period) && m_displacementScale > 0.0f)
		{
			m_displacementScale -= elapsedTime;
			m_displacementScale = math::Max(m_displacementScale, 0.0f);
		}
	}
}

void DistortionMappingDemo::UpdateDistortionMapMaterial()
{
	m_material->GetSceneTexture() << GetSceneTexture();
	m_material->GetDistortionMapTexture() << m_distortionMapTexture.Get();
	m_material->GetDisplacementScale() << m_displacementScale;
}
