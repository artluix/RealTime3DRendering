#include "SpotlightDemo.h"

#include "DemoUtils.h"

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

	CreateMaterialWithEffect("Lights");

	// load model
	{
		Model model(GetApp(), "Plane", true);
		m_primitivesData = m_material->CreatePrimitivesData(GetApp().GetDevice(), model);
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().CreateTexture2DSRV("Checkerboard.png");

	m_spotlight = std::make_unique<SpotlightComponent>();
	m_spotlight->SetupProxyModel(*GetCamera());
	m_spotlight->SetRadius(10.f);
	m_spotlight->SetPosition(math::Vector3(0.0f, 0.f, 0.5f));

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
	if (!!m_keyboard)
	{
		const auto& keyboard = *m_keyboard;

		UpdateAmbientLight(time, keyboard);
		UpdateSpotlight(time, keyboard);
		UpdateSpecularLight(time, keyboard);
	}

	m_text->Update(time);
	m_spotlight->Update(time);

	PrimitiveComponent::Update(time);
}

void SpotlightDemo::UpdateAmbientLight(const Time& time, const library::KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_ambientColor.a, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::PageUp, Key::PageDown));
}

void SpotlightDemo::UpdateSpotlight(const Time& time, const library::KeyboardComponent& keyboard)
{
	const auto elapsedTime = time.elapsed.GetSeconds();
	const float modulationStepValue = elapsedTime * k_lightModulationRate;

	// update light color intensity
	{
		auto lightColor = m_spotlight->GetColor();
		if (::utils::UpdateValue(lightColor.a, modulationStepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::Home, Key::End)))
		{
			m_spotlight->SetColor(lightColor);
		}
	}

	// move
	{
		math::Vector3i movementAmount;

		if (keyboard.IsKeyDown(Key::Num_4))
			movementAmount.x--;
		if (keyboard.IsKeyDown(Key::Num_6))
			movementAmount.x++;

		if (keyboard.IsKeyDown(Key::Num_9))
			movementAmount.y++;
		if (keyboard.IsKeyDown(Key::Num_3))
			movementAmount.y--;

		if (keyboard.IsKeyDown(Key::Num_8))
			movementAmount.z--;
		if (keyboard.IsKeyDown(Key::Num_2))
			movementAmount.z++;

		if (movementAmount)
		{
			const auto movement = movementAmount * k_lightMovementRate * elapsedTime;
			m_spotlight->SetPosition(m_spotlight->GetPosition() + movement);
		}
	}

	// rotate
	{
		math::Vector2 rotationAmount;

		if (keyboard.IsKeyDown(Key::Left))
			rotationAmount.x += k_lightRotationRate.x * elapsedTime;

		if (keyboard.IsKeyDown(Key::Right))
			rotationAmount.x -= k_lightRotationRate.x * elapsedTime;

		if (keyboard.IsKeyDown(Key::Up))
			rotationAmount.y += k_lightRotationRate.y * elapsedTime;

		if (keyboard.IsKeyDown(Key::Down))
			rotationAmount.y -= k_lightRotationRate.y * elapsedTime;

		if (rotationAmount)
		{
			const auto rotation = math::Quaternion::RotationPitchYawRoll(rotationAmount.y, rotationAmount.x, 0.f);
			m_spotlight->Rotate(rotation);
		}
	}

	// radius
	{
		auto radius = m_spotlight->GetRadius();
		if (::utils::UpdateValue(radius, modulationStepValue, math::Interval(0.f, 50.f), keyboard, KeyPair(Key::B, Key::N)))
		{
			m_spotlight->SetRadius(radius);
		}
	}

	// inner and outer angles
	{
		auto innerAngle = m_spotlight->GetInnerAngle();
		if (::utils::UpdateValue(innerAngle, elapsedTime, math::Interval(.5f, 1.f), keyboard, KeyPair(Key::Z, Key::X)))
		{
			m_spotlight->SetInnerAngle(innerAngle);
		}

		auto outerAngle = m_spotlight->GetOuterAngle();
		if (::utils::UpdateValue(outerAngle, elapsedTime, math::Interval(.0f, .5f), keyboard, KeyPair(Key::C, Key::V)))
		{
			m_spotlight->SetOuterAngle(outerAngle);
		}
	}
}

void SpotlightDemo::UpdateSpecularLight(const Time& time, const library::KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_specularPower, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::Insert, Key::Delete));
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

	const auto isLightVisible = m_spotlight->IsVisibleFrom(GetPosition());
	m_material->GetSpotlightsCount() << unsigned(isLightVisible);
	if (isLightVisible)
		m_material->GetSpotlights() << SpotlightData(*m_spotlight);

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
