#include "SpotlightDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>
#include <library/Components/SpotlightComponent.h>
#include <library/Components/ProxyModelComponent.h>
#include <library/Components/TextComponent.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>

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
constexpr float k_lightModulationRate = 10.f;
constexpr float k_lightAngleDelta = 5.f;
constexpr float k_lightMovementRate = 10.f;
constexpr auto k_lightRotationRate = math::Vector2(math::Pi_2);

constexpr auto k_proxyModelRotationOffset = math::Vector3(math::Pi_Div_2, 0.f, 0.f);
} // namespace

//-------------------------------------------------------------------------

SpotlightDemo::SpotlightDemo()
	: m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)
{
}

SpotlightDemo::~SpotlightDemo() = default;

//-------------------------------------------------------------------------

void SpotlightDemo::InitializeInternal()
{
	assert(!!GetCamera());

	CreateMaterialWithEffect("Spotlight");

	// load model
	{
		Model model(GetApp(), "Plane", true);
		m_primitivesData = m_material->CreatePrimitivesData(GetApp().GetDevice(), model);
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().LoadTexture("Checkerboard");

	m_spotlight = std::make_unique<SpotlightComponent>();
	m_spotlight->SetRadius(10.f);
	m_spotlight->SetPosition(math::Vector3(0.0f, 0.f, 0.5f));

	m_proxyModel = std::make_unique<ProxyModelComponent>("SpotlightProxy", 0.3f);
	m_proxyModel->SetCamera(*GetCamera());
	m_proxyModel->SetInitialTransform(math::Matrix4::RotationPitchYawRoll(k_proxyModelRotationOffset));
	m_proxyModel->SetPosition(m_spotlight->GetPosition());
	m_proxyModel->Initialize(GetApp());

	m_spotlight2 = std::make_unique<SpotlightComponent>();
	m_spotlight2->SetRadius(10.f);
	m_spotlight2->SetPosition(math::Vector3(1.0f, 0.f, 0.5f));

	m_proxyModel2 = std::make_unique<ProxyModelComponent>("SpotlightProxy", 0.3f);
	m_proxyModel2->SetCamera(*GetCamera());
	m_proxyModel2->SetInitialTransform(math::Matrix4::RotationPitchYawRoll(k_proxyModelRotationOffset));
	m_proxyModel2->SetPosition(m_spotlight2->GetPosition());
	m_proxyModel2->Initialize(GetApp());

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		std::wostringstream woss;
		woss << L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n"
			 << L"Spotlight Intensity (+Home/-End): " << m_spotlight->GetColor().a << "\n"
			 << L"Specular Power (+Insert/-Delete): " << m_specularPower << "\n"
			 << L"Spotlight Radius (+B/-N): " << m_spotlight->GetRadius() << "\n"
			 << L"Spotlight Inner Angle (+Z/-X): " << m_spotlight->GetInnerAngle() << "\n"
			 << L"Spotlight Outer Angle (+C/-V): " << m_spotlight->GetOuterAngle() << "\n"
			 << L"Move Spotlight (Numpad: 8/2, 4/6, 3/9)\n"
			 << L"Rotate Spotlight (Arrow Keys)\n";

		return woss.str();
	});
	m_text->Initialize(GetApp());

	SetRotation(math::Vector3(math::Pi_Div_2, 0.f, 0.f));
	SetScaling(math::Vector3(5.f));
}

void SpotlightDemo::Update(const Time& time)
{
	UpdateAmbientLight(time);
	UpdateSpotlight(time);
	UpdateSpecularLight(time);

	m_text->Update(time);
	m_proxyModel->Update(time);
	m_proxyModel2->Update(time);

	PrimitiveComponent::Update(time);
}

void SpotlightDemo::UpdateAmbientLight(const Time& time)
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

void SpotlightDemo::UpdateSpotlight(const Time& time)
{
	static float spotlightIntensity = m_spotlight->GetColor().a;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		// update directional light intensity
		if (m_keyboard->IsKeyDown(Key::Home) && spotlightIntensity < k_byteMax)
		{
			spotlightIntensity += k_lightModulationRate * elapsedTime;

			auto spotightColor = m_spotlight->GetColor();
			spotightColor.a = math::Min(spotlightIntensity, k_byteMax);
			m_spotlight->SetColor(spotightColor);
		}

		if (m_keyboard->IsKeyDown(Key::End) && spotlightIntensity > 0)
		{
			spotlightIntensity -= k_lightModulationRate * elapsedTime;

			auto spotlightColor = m_spotlight->GetColor();
			spotlightColor.a = math::Max(spotlightIntensity, 0.f);
			m_spotlight->SetColor(spotlightColor);
		}

		// move
		{
			math::Vector3i movementAmount;

			if (m_keyboard->IsKeyDown(Key::Num_4))
				movementAmount.x--;
			if (m_keyboard->IsKeyDown(Key::Num_6))
				movementAmount.x++;

			if (m_keyboard->IsKeyDown(Key::Num_9))
				movementAmount.y++;
			if (m_keyboard->IsKeyDown(Key::Num_3))
				movementAmount.y--;

			if (m_keyboard->IsKeyDown(Key::Num_8))
				movementAmount.z--;
			if (m_keyboard->IsKeyDown(Key::Num_2))
				movementAmount.z++;

			if (movementAmount)
			{
				auto movement = movementAmount * k_lightMovementRate * elapsedTime;

				const auto rotationMatrix = math::Matrix4::RotationQuaternion(m_proxyModel->GetRotation());
				movement = movement.Transform(rotationMatrix);

				m_spotlight->SetPosition(m_spotlight->GetPosition() + movement);
				m_proxyModel->SetPosition(m_spotlight->GetPosition() + movement);
			}
		}

		// rotate
		{
			math::Vector2 rotationAmount;

			if (m_keyboard->IsKeyDown(Key::Left))
				rotationAmount.x += k_lightRotationRate.x * elapsedTime;

			if (m_keyboard->IsKeyDown(Key::Right))
				rotationAmount.x -= k_lightRotationRate.x * elapsedTime;

			if (m_keyboard->IsKeyDown(Key::Up))
				rotationAmount.y += k_lightRotationRate.y * elapsedTime;

			if (m_keyboard->IsKeyDown(Key::Down))
				rotationAmount.y -= k_lightRotationRate.y * elapsedTime;

			if (rotationAmount)
			{
				const auto rotation = math::Quaternion::RotationPitchYawRoll(rotationAmount.y, rotationAmount.x, 0.f);
				m_spotlight->Rotate(rotation);
				m_proxyModel->Rotate(rotation);
			}
		}

		// radius
		{
			if (m_keyboard->IsKeyDown(Key::B))
			{
				float radius = m_spotlight->GetRadius() + k_lightModulationRate * elapsedTime;
				m_spotlight->SetRadius(radius);
			}

			if (m_keyboard->IsKeyDown(Key::N))
			{
				float radius = m_spotlight->GetRadius() - k_lightModulationRate * elapsedTime;
				radius = math::Max(radius, 0.0f);
				m_spotlight->SetRadius(radius);
			}
		}

		// inner and outer angles
		{
			static float innerAngle = m_spotlight->GetInnerAngle();
			if (m_keyboard->IsKeyDown(Key::Z) && innerAngle < 1.0f)
			{
				innerAngle += elapsedTime;
				innerAngle = math::Min(innerAngle, 1.0f);

				m_spotlight->SetInnerAngle(innerAngle);
			}
			if (m_keyboard->IsKeyDown(Key::X) && innerAngle > 0.5f)
			{
				innerAngle -= elapsedTime;
				innerAngle = math::Max(innerAngle, 0.5f);

				m_spotlight->SetInnerAngle(innerAngle);
			}

			static float outerAngle = m_spotlight->GetOuterAngle();
			if (m_keyboard->IsKeyDown(Key::C) && outerAngle < 0.5f)
			{
				outerAngle += elapsedTime;
				outerAngle = math::Min(outerAngle, 0.5f);

				m_spotlight->SetOuterAngle(outerAngle);
			}
			if (m_keyboard->IsKeyDown(Key::V) && outerAngle > 0.0f)
			{
				outerAngle -= elapsedTime;
				outerAngle = math::Max(outerAngle, 0.0f);

				m_spotlight->SetOuterAngle(outerAngle);
			}
		}
	}

	// Cull light
	{
		m_lightsData.spotlightsCount = 0;

		const auto lightDirection = m_spotlight->GetPosition() - GetPosition();
		m_lightsData.spotlightsCount += unsigned(lightDirection.Length() < m_spotlight->GetRadius());

		if (m_lightsData.spotlightsCount)
		{
			m_lightsData.spotlights[0] = m_spotlight->GetData();
		}

		m_lightsData.spotlightsCount++;
		m_lightsData.spotlights[1] = m_spotlight2->GetData();
	}
}

void SpotlightDemo::UpdateSpecularLight(const Time& time)
{
	static float specularLightIntensity = m_specularPower;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		if (m_keyboard->IsKeyDown(Key::Insert) && specularLightIntensity < k_byteMax)
		{
			specularLightIntensity += k_lightModulationRate * elapsedTime;
			specularLightIntensity = math::Min(specularLightIntensity, k_byteMax);

			m_specularPower = specularLightIntensity;
		}

		if (m_keyboard->IsKeyDown(Key::Delete) && specularLightIntensity > 0.f)
		{
			specularLightIntensity -= k_lightModulationRate * elapsedTime;
			specularLightIntensity = math::Max(specularLightIntensity, 0.f);

			m_specularPower = specularLightIntensity;
		}
	}
}

void SpotlightDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	const auto& world = GetWorldMatrix();
	auto wvp = world;

	if (auto camera = GetCamera())
	{
		wvp *= camera->GetViewProjectionMatrix();

		m_material->GetCameraPosition() << camera->GetPosition();
	}

	m_material->GetWVP() << wvp;
	m_material->GetWorld() << world;
	m_material->GetSpecularPower() << m_specularPower;
	m_material->GetSpecularColor() << m_specularColor.ToVector4();
	m_material->GetAmbientColor() << m_ambientColor.ToVector4();

	m_material->GetLightData() << m_spotlight->GetData();
	m_material->GetLightsData() << m_lightsData;

	constexpr auto b = sizeof(LightsData);

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
