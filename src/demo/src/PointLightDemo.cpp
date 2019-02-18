#include "PointLightDemo.h"

#include <library/components/CameraComponent.h>
#include <library/components/KeyboardComponent.h>
#include <library/components/PointLightComponent.h>
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
	constexpr float k_byteMax = static_cast<float>(0xFF);
	constexpr float k_lightModulationRate = 10.f;
	constexpr float k_lightMovementRate = 10.f;
}

PointLightDemo::PointLightDemo()
	: m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)
{
	SetDefaultModelName("Sphere");
	SetDefaultTextureName("EarthAtDay");
}

PointLightDemo::~PointLightDemo() = default;

void PointLightDemo::Initialize(const Application& app)
{
	assert(!!GetCamera());

	InitializeMaterial(app, "PointLight");
	DrawableInputMaterialComponent::Initialize(app);

	m_pointLight = std::make_unique<PointLightComponent>();
	m_pointLight->SetRadius(500.f);
	m_pointLight->SetPosition(math::Vector3(0.f, 0.f, 10.f));

	m_proxyModel = std::make_unique<ProxyModelComponent>("PointLightProxy", 0.5f);
	m_proxyModel->SetPosition(m_pointLight->GetPosition());
	m_proxyModel->Rotate(math::Vector3(0.f, math::Pi_Div_2, 0.f));
	m_proxyModel->SetCamera(*GetCamera());
	m_proxyModel->Initialize(app);

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextGeneratorFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss <<
				L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n" <<
				L"Point Light Intensity (+Home/-End): " << m_pointLight->GetColor().a << "\n" <<
				L"Specular Power (+Insert/-Delete): " << m_specularPower << "\n" <<
				L"Move Point Light (Numpad: 8/2, 4/6, 3/9)\n";
			return woss.str();
		}
	);
	m_text->Initialize(app);
}

void PointLightDemo::Update(const Time& time)
{
	UpdateAmbientLight(time);
	UpdatePointLight(time);
	UpdateSpecularLight(time);

	m_text->Update(time);
	m_proxyModel->Update(time);

	DrawableComponent::Update(time);
}

void PointLightDemo::UpdateAmbientLight(const Time& time)
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

void PointLightDemo::UpdatePointLight(const Time& time)
{
	static float pointLightIntensity = m_pointLight->GetColor().a;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		// update directional light intensity
		if (m_keyboard->IsKeyDown(Key::Home) && pointLightIntensity < k_byteMax)
		{
			pointLightIntensity += k_lightModulationRate * elapsedTime;

			auto pointLightColor = m_pointLight->GetColor();
			pointLightColor.a = math::Min(pointLightIntensity, k_byteMax);
			m_pointLight->SetColor(pointLightColor);
		}

		if (m_keyboard->IsKeyDown(Key::End) && pointLightIntensity > 0)
		{
			pointLightIntensity -= k_lightModulationRate * elapsedTime;

			auto pointLightColor = m_pointLight->GetColor();
			pointLightColor.a = math::Max(pointLightIntensity, 0.f);
			m_pointLight->SetColor(pointLightColor);
		}

		math::Vector3 movementAmount;

		if (m_keyboard->IsKeyDown(Key::Num_4))
			movementAmount.x = -1.0f;
		if (m_keyboard->IsKeyDown(Key::Num_6))
			movementAmount.x = 1.0f;

		if (m_keyboard->IsKeyDown(Key::Num_9))
			movementAmount.y = 1.0f;
		if (m_keyboard->IsKeyDown(Key::Num_3))
			movementAmount.y = -1.0f;

		if (m_keyboard->IsKeyDown(Key::Num_8))
			movementAmount.z = -1.0f;
		if (m_keyboard->IsKeyDown(Key::Num_2))
			movementAmount.z = 1.0f;

		if (movementAmount)
		{
			auto movement = movementAmount * k_lightMovementRate * elapsedTime;

			m_pointLight->SetPosition(m_pointLight->GetPosition() + movement);
			m_proxyModel->Translate(movement);
		}
	}
}

void PointLightDemo::UpdateSpecularLight(const Time& time)
{
	static float specularLightIntensity = m_specularPower;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		if (m_keyboard->IsKeyDown(Key::Insert) && specularLightIntensity < k_byteMax)
		{
			specularLightIntensity += k_lightModulationRate * elapsedTime;
			specularLightIntensity = math::Min(specularLightIntensity, k_byteMax);

			m_specularPower = specularLightIntensity;;
		}

		if (m_keyboard->IsKeyDown(Key::Delete) && specularLightIntensity > 0)
		{
			specularLightIntensity -= k_lightModulationRate * elapsedTime;
			specularLightIntensity = math::Max(specularLightIntensity, 0.f);

			m_specularPower = specularLightIntensity;
		}
	}
}

void PointLightDemo::Draw_SetData()
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
	{
		wvp *= camera->GetViewProjectionMatrix();

		m_material->GetCameraPosition() << camera->GetPosition();
	}

	m_material->GetWVP() << wvp;
	m_material->GetWorld() << GetWorldMatrix();
	m_material->GetSpecularPower() << m_specularPower;
	m_material->GetSpecularColor() << m_specularColor;
	m_material->GetAmbientColor() << m_ambientColor;
	m_material->GetLightColor() << m_pointLight->GetColor();
	m_material->GetLightPosition() << m_pointLight->GetPosition();
	m_material->GetLightRadius() << m_pointLight->GetRadius();
	m_material->GetColorTexture() << m_defaultTexture.Get();

	DrawableInputMaterialComponent::Draw_SetData();
}