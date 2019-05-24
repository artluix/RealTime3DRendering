#include "DiffuseLightingDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>
#include <library/Components/DirectionalLightComponent.h>
#include <library/Components/ProxyModelComponent.h>
#include <library/Components/TextComponent.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>
#include <library/Exception.h>
#include <library/Math/Math.h>

#include <library/Model/Model.h>

#include <library/Effect/Effect.h>
#include <library/Effect/EffectPass.h>
#include <library/Effect/EffectTechnique.h>
#include <library/Effect/EffectVariable.h>

#include <sstream>

using namespace library;

namespace
{
constexpr float k_byteMax = static_cast<float>(0xFF);

constexpr float k_lightModulationRate = k_byteMax / 40;
constexpr auto k_lightRotationRate = math::Vector2(math::Pi);

constexpr auto k_proxyModelRotationOffset = math::Vector3(0.f, math::Pi_Div_2, 0.f);
constexpr float k_proxyModelDistanceOffset = 10.f;
} // namespace

//-------------------------------------------------------------------------

DiffuseLightingDemo::DiffuseLightingDemo() : m_ambientColor(1.f, 1.f, 1.f, 0.f)
{}

DiffuseLightingDemo::~DiffuseLightingDemo() = default;

//-------------------------------------------------------------------------

void DiffuseLightingDemo::InitializeInternal()
{
	assert(!!GetCamera());

	InitializeMaterial("DiffuseLighting");

	Model model(GetApp(), "Sphere", true);
	m_primitivesData = GetMaterial()->CreatePrimitivesData(GetApp().GetDevice(), model);

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().LoadTexture("EarthComposite");

	m_directionalLight = std::make_unique<DirectionalLightComponent>();

	m_proxyModel = std::make_unique<ProxyModelComponent>("DirectionalLightProxy", 0.5f);
	m_proxyModel->SetPosition(
		GetPosition() + -m_directionalLight->GetDirection() * k_proxyModelDistanceOffset);
	m_proxyModel->SetInitialTransform(math::Matrix4::RotationPitchYawRoll(k_proxyModelRotationOffset));
	m_proxyModel->SetCamera(*GetCamera());
	m_proxyModel->Initialize(GetApp());

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		std::wostringstream woss;
		woss << L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n"
			 << L"Directional Light Intensity (+Home/-End): " << m_directionalLight->GetColor().a << "\n"
			 << L"Rotate Directional Light (Arrow Keys)\n";
		return woss.str();
	});
	m_text->Component::Initialize(GetApp());
}

void DiffuseLightingDemo::Update(const Time& time)
{
	UpdateDirectionalLight(time);
	UpdateAmbientLight(time);

	m_text->Update(time);
	m_proxyModel->Update(time);

	PrimitiveComponent::Update(time);
}

void DiffuseLightingDemo::UpdateAmbientLight(const Time& time)
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

void DiffuseLightingDemo::UpdateDirectionalLight(const Time& time)
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
			m_directionalLight->Rotate(math::Quaternion::RotationPitchYawRoll(
				math::Vector3(rotationAmount.y, rotationAmount.x, 0.f)));

			m_proxyModel->SetPosition(
				GetPosition() + -m_directionalLight->GetDirection() * k_proxyModelDistanceOffset);
			m_proxyModel->SetRotation(m_directionalLight->GetRotation());
		}
	}
}

void DiffuseLightingDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();

	m_material->GetWVP() << math::XMMatrix(wvp);
	m_material->GetWorld() << math::XMMatrix(GetWorldMatrix());
	m_material->GetAmbientColor() << math::XMVector(m_ambientColor);
	m_material->GetLightColor() << math::XMVector(m_directionalLight->GetColor());
	m_material->GetLightDirection() << math::XMVector(m_directionalLight->GetDirection());
	m_material->GetColorTexture() << m_textures[Texture::Default].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
