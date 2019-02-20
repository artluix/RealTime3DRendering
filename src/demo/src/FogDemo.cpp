#include "FogDemo.h"

#include <library/components/CameraComponent.h>
#include <library/components/KeyboardComponent.h>
#include <library/components/DirectionalLightComponent.h>
#include <library/components/ProxyModelComponent.h>
#include <library/components/TextComponent.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>
#include <library/Exception.h>

#include <library/effect/Effect.h>
#include <library/effect/EffectPass.h>
#include <library/effect/EffectTechnique.h>
#include <library/effect/EffectVariable.h>

#include <sstream>

using namespace library;

namespace
{
	const auto k_fogColor = Color::CornFlower;

	constexpr float k_byteMax = static_cast<float>(0xFF);

	constexpr float k_lightModulationRate = k_byteMax / 40;
	constexpr auto k_lightRotationRate = math::Vector2(math::Pi_2);

	constexpr auto k_proxyModelRotationOffset = math::Vector3(0.f, math::Pi_Div_2, 0.f);
	constexpr float k_proxyModelDistanceOffset = 10.f;

	constexpr auto k_rotationOffset = math::Vector3(0.f, math::Pi_Div_2, 0.f);
}

//-------------------------------------------------------------------------

FogDemo::FogDemo()
	: m_ambientColor(1.f, 1.f, 1.f, 0.f)
	, m_fogStart(15.0f)
	, m_fogRange(20.0f)
	, m_fogEnabled(true)
{
	SetModelName("Sphere");
	SetTextureName("EarthComposite");
}

FogDemo::~FogDemo() = default;

//-------------------------------------------------------------------------

void FogDemo::Initialize(const Application& app)
{
	assert(!!GetCamera());

	InitializeMaterial(app, "Fog");
	MaterialSceneComponent::Initialize(app);

	m_directionalLight = std::make_unique<DirectionalLightComponent>();

	m_proxyModel = std::make_unique<ProxyModelComponent>("DirectionalLightProxy", 0.2f);
	m_proxyModel->SetPosition(GetPosition() + -m_directionalLight->GetDirection() * k_proxyModelDistanceOffset);
	m_proxyModel->SetRotation(GetRotation() + k_proxyModelRotationOffset);
	m_proxyModel->SetCamera(*GetCamera());
	m_proxyModel->Initialize(app);

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss <<
				L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n" <<
				L"Directional Light Intensity (+Home/-End): " << m_directionalLight->GetColor().a << "\n" <<
				L"Rotate Directional Light (Arrow Keys)\n" <<
				L"Fog enabled (Space): " << (m_fogEnabled ? "Yes" : "No") << "\n" <<
				L"Fog start: " << m_fogStart << "\n" <<
				L"Fog range: " << m_fogRange << "\n";
			return woss.str();
		}
	);
	m_text->Initialize(app);

	SetActiveTechnique();
}

void FogDemo::Update(const Time& time)
{
	if (!!m_keyboard && m_keyboard->WasKeyReleased(Key::Space))
	{
		m_fogEnabled = !m_fogEnabled;
		SetActiveTechnique();
	}

	UpdateDirectionalLight(time);
	UpdateAmbientLight(time);

	m_text->Update(time);
	m_proxyModel->Update(time);

	MaterialSceneComponent::Update(time);
}

void FogDemo::UpdateAmbientLight(const Time& time)
{
	static float ambientLightIntensity = m_ambientColor.a;

	if (!!m_keyboard)
	{
		if (m_keyboard->IsKeyDown(Key::PageUp) && ambientLightIntensity < k_byteMax)
		{
			ambientLightIntensity += k_lightModulationRate * time.elapsed.GetSeconds();
			m_ambientColor.a = math::Min(ambientLightIntensity, k_byteMax);
		}

		if (m_keyboard->IsKeyDown(Key::PageDown) && ambientLightIntensity > 0)
		{
			ambientLightIntensity -= k_lightModulationRate * time.elapsed.GetSeconds();
			m_ambientColor.a = math::Max(ambientLightIntensity, 0.f);
		}
	}
}

void FogDemo::UpdateDirectionalLight(const Time& time)
{
	static float directionalLightIntensity = m_directionalLight->GetColor().a;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		// update directional light intensity
		if (m_keyboard->IsKeyDown(Key::Home) && directionalLightIntensity < k_byteMax)
		{
			directionalLightIntensity += k_lightModulationRate * elapsedTime;

			auto directionalLightColor = m_directionalLight->GetColor();
			directionalLightColor.a = math::Min(directionalLightIntensity, k_byteMax);
			m_directionalLight->SetColor(directionalLightColor);
		}

		if (m_keyboard->IsKeyDown(Key::End) && directionalLightIntensity > 0)
		{
			directionalLightIntensity -= k_lightModulationRate * elapsedTime;

			auto directionalLightColor = m_directionalLight->GetColor();
			directionalLightColor.a = math::Max(directionalLightIntensity, 0.f);
			m_directionalLight->SetColor(directionalLightColor);
		}

		// rotate directional light
		math::Vector2 rotationAmount;
		if (m_keyboard->IsKeyDown(Key::Left))
			rotationAmount.x -= k_lightRotationRate.x * elapsedTime;

		if (m_keyboard->IsKeyDown(Key::Right))
			rotationAmount.x += k_lightRotationRate.x * elapsedTime;

		if (m_keyboard->IsKeyDown(Key::Up))
			rotationAmount.y -= k_lightRotationRate.y * elapsedTime;

		if (m_keyboard->IsKeyDown(Key::Down))
			rotationAmount.y += k_lightRotationRate.y * elapsedTime;

		if (rotationAmount)
		{
			m_directionalLight->Rotate(math::Vector3(rotationAmount.y, rotationAmount.x, 0.f));

			const auto dirLightRot = m_directionalLight->GetRotation();

			m_proxyModel->SetPosition(GetPosition() + -m_directionalLight->GetDirection() * k_proxyModelDistanceOffset);
			m_proxyModel->SetRotation(math::Vector3(dirLightRot.z, dirLightRot.y, dirLightRot.x) + k_proxyModelRotationOffset);
		}
	}
}

void FogDemo::SetActiveTechnique()
{
	const std::string techniqueName = m_fogEnabled ? "fogEnabled" : "fogDisabled";
	const auto& technique = m_material->GetEffect().GetTechnique(techniqueName);
	const auto& pass = technique.GetPass("p0");

	m_input.layout = m_material->GetInputLayout(pass);
}

void FogDemo::Draw_SetData()
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
	{
		wvp *= camera->GetViewProjectionMatrix();

		m_material->GetCameraPosition() << camera->GetPosition();
	}

	m_material->GetAmbientColor() << m_ambientColor;
	m_material->GetLightColor() << m_directionalLight->GetColor();
	m_material->GetLightDirection() << m_directionalLight->GetDirection();
	m_material->GetFogColor() << k_fogColor;
	m_material->GetFogStart() << m_fogStart;
	m_material->GetFogRange() << m_fogRange;

	m_material->GetWVP() << wvp;
	m_material->GetWorld() << GetWorldMatrix();

	m_material->GetColorTexture() << GetTexture();

	MaterialSceneComponent::Draw_SetData();
}
