#include "Components/MultipleLightsDemo.h"

#include "DemoUtils.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>
#include <library/Components/PointLightComponent.h>
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
constexpr float k_lightMovementRate = 10.f;

constexpr unsigned k_minLightsCount = 1;
constexpr unsigned k_maxLightsCount = 4;

constexpr auto k_backgroundColor = math::Color(0.f);
} // namespace

MultipleLightsDemo::MultipleLightsDemo()
	: m_lightsCount(1)
	, m_lightColor(colors::White)

	, m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)
{
}

void MultipleLightsDemo::InitializeInternal()
{
	assert(!!GetCamera());

	CreateMaterialWithEffect("PhongLights");

	// load model
	{
		Model model(GetApp(), "Sphere", true);
		CreatePrimitivesData(model);
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().CreateTexture2DSRV("EarthAtDay.tga");

	m_lights.reserve(k_maxLightsCount);
	for (unsigned i = 0; i < k_maxLightsCount; i++)
	{
		auto pointLightComponent = std::make_unique<PointLightComponent>();
		pointLightComponent->SetupProxyModel(*GetCamera());
		pointLightComponent->SetRadius(500.f);

		m_lights.emplace_back(std::move(pointLightComponent));
	}

	// set position for lights
	m_lights[0]->SetPosition(math::Vector3(0.f, -10.f, 10.f));
	m_lights[1]->SetPosition(math::Vector3(0.f, 10.f, 10.f));
	m_lights[2]->SetPosition(math::Vector3(-10.f, 0.f, 0.f));
	m_lights[3]->SetPosition(math::Vector3(10.f, 0.f, 0.f));

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		std::wostringstream woss;
		woss << L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n"
			 << L"Point Light Intensity (+Home/-End): " << m_lightColor.a << "\n"
			 << L"Specular Power (+Insert/-Delete): " << m_specularPower << "\n"
			 << L"Lights Count (+Right Alt/-Left Alt): " << m_lightsCount << "\n"
			 << L"Move Point Light (Numpad: 8/2, 4/6, 3/9)\n";
		return woss.str();
	});
	m_text->Initialize(GetApp());
}

//-------------------------------------------------------------------------

void MultipleLightsDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto& keyboard = *m_keyboard;

		if (keyboard.WasKeyPressed(Key::Alt_Left) && m_lightsCount > k_minLightsCount)
			m_lightsCount--;
		if (keyboard.WasKeyPressed(Key::Alt_Right) && m_lightsCount < k_maxLightsCount)
			m_lightsCount++;

		UpdateAmbientLight(time, keyboard);
		UpdatePointLight(time, keyboard);
		UpdateSpecularLight(time, keyboard);
	}

	m_text->Update(time);

	for (unsigned i = 0; i < m_lightsCount; i++)
	{
		m_lights[i]->Update(time);
	}

	PrimitiveComponent::Update(time);
}

void MultipleLightsDemo::UpdateAmbientLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_ambientColor.a, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::PageUp, Key::PageDown));
}

void MultipleLightsDemo::UpdatePointLight(const Time& time, const KeyboardComponent& keyboard)
{
	const auto elapsedTime = time.elapsed.GetSeconds();

	// update lights color intensity
	{
		const float modulationStepValue = elapsedTime * k_lightModulationRate;

		if (::utils::UpdateValue(m_lightColor.a, modulationStepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::Home, Key::End)))
		{
			for (unsigned i = 0; i < m_lightsCount; i++)
			{
				m_lights[i]->SetColor(m_lightColor);
			}
		}
	}

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

		for (unsigned i = 0; i < m_lightsCount; i++)
		{
			const auto position = m_lights[i]->GetPosition() + movement;

			m_lights[i]->SetPosition(position);
		}
	}
}

void MultipleLightsDemo::UpdateSpecularLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_specularPower, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::Insert, Key::Delete));
}

//-------------------------------------------------------------------------

void MultipleLightsDemo::Draw_SetData(const PrimitiveData& primitiveData)
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
	m_material->GetColorTexture() << m_textures[Texture::Default].Get();

	DynArray<PointLightData> pointLightsData;
	for (unsigned i = 0; i < m_lightsCount; i++)
	{
		const auto& light = *m_lights[i];
		const auto isLightVisible = light.IsVisibleFrom(GetPosition());
		if (isLightVisible)
			pointLightsData.emplace_back(PointLightData(light));
	}
	m_material->GetPointLightsCount() << unsigned(pointLightsData.size());
	m_material->GetPointLights() << pointLightsData;

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
