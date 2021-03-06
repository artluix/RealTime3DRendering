#include "PointLightDemo.h"

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
} // namespace

PointLightDemo::PointLightDemo()
	: m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)
{}

PointLightDemo::~PointLightDemo() = default;

//-------------------------------------------------------------------------

void PointLightDemo::InitializeInternal()
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
	//m_textures[Texture::Default] = GetApp().CreateTexture2DSRV("EarthComposite.jpg");

	m_pointLight = std::make_unique<PointLightComponent>();
	m_pointLight->SetupProxyModel(*GetCamera());
	m_pointLight->SetRadius(500.f);
	m_pointLight->SetPosition(math::Vector3(0.f, 0.f, 10.f));

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		std::wostringstream woss;
		woss << L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n"
			 << L"Point Light Intensity (+Home/-End): " << m_pointLight->GetColor().a << "\n"
			 << L"Specular Power (+Insert/-Delete): " << m_specularPower << "\n"
			 << L"Move Point Light (Numpad: 8/2, 4/6, 3/9)\n";
		return woss.str();
	});
	m_text->Initialize(GetApp());
}

void PointLightDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto& keyboard = *m_keyboard;

		UpdateAmbientLight(time, keyboard);
		UpdatePointLight(time, keyboard);
		UpdateSpecularLight(time, keyboard);
	}

	m_text->Update(time);
	m_pointLight->Update(time);

	PrimitiveComponent::Update(time);
}

void PointLightDemo::UpdateAmbientLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_ambientColor.a, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::PageUp, Key::PageDown));
}

void PointLightDemo::UpdatePointLight(const Time& time, const KeyboardComponent& keyboard)
{
	const auto elapsedTime = time.elapsed.GetSeconds();

	// update light color intensity
	{
		const float modulationStepValue = elapsedTime * k_lightModulationRate;

		auto lightColor = m_pointLight->GetColor();
		if (::utils::UpdateValue(lightColor.a, modulationStepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::Home, Key::End)))
		{
			m_pointLight->SetColor(lightColor);
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
		const auto position = m_pointLight->GetPosition() + movement;

		m_pointLight->SetPosition(position);
	}
}

void PointLightDemo::UpdateSpecularLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_specularPower, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::Insert, Key::Delete));
}

void PointLightDemo::Draw_SetData(const PrimitiveData& primitiveData)
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
	m_material->GetSpecularColor() << m_specularColor.ToVector();
	m_material->GetAmbientColor() << m_ambientColor.ToVector();

	const auto isLightVisible = m_pointLight->IsVisibleFrom(GetPosition());
	m_material->GetPointLightsCount() << unsigned(isLightVisible);
	if (isLightVisible)
		m_material->GetPointLights() << PointLightData(*m_pointLight);

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
