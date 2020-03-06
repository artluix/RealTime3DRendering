#include "DiffuseLightingDemo.h"

#include "LightsData.h"
#include "DemoUtils.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>
#include <library/Components/DirectionalLightComponent.h>
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

constexpr float k_lightModulationRate = k_byteMax / 40;
constexpr auto k_lightRotationRate = math::Vector2(math::Pi);

constexpr auto k_proxyModelRotationOffset = math::Vector3(0.f, math::Pi_Div_2, 0.f);
} // namespace

//-------------------------------------------------------------------------

DiffuseLightingDemo::DiffuseLightingDemo() : m_ambientColor(1.f, 1.f, 1.f, 0.f)
{}

DiffuseLightingDemo::~DiffuseLightingDemo() = default;

//-------------------------------------------------------------------------

void DiffuseLightingDemo::InitializeInternal()
{
	assert(!!GetCamera());

	CreateMaterialWithEffect("DiffuseLighting");

	// load model
	{
		Model model(GetApp(), "Sphere", true);
		m_primitivesData = m_material->CreatePrimitivesData(GetApp().GetDevice(), model);
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().CreateTexture2DSRV("EarthComposite.jpg");

	m_directionalLight = std::make_unique<DirectionalLightComponent>();
	m_directionalLight->SetupProxyModel(*GetCamera());
	m_directionalLight->GetProxyModel()->SetPosition(GetPosition() + math::Vector3(5.f));

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
	if (!!m_keyboard)
	{
		const auto& keyboard = *m_keyboard;

		UpdateDirectionalLight(time, keyboard);
		UpdateAmbientLight(time, keyboard);
	}

	m_text->Update(time);
	m_directionalLight->Update(time);

	PrimitiveComponent::Update(time);
}

void DiffuseLightingDemo::UpdateAmbientLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_ambientColor.a, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::PageUp, Key::PageDown));
}

void DiffuseLightingDemo::UpdateDirectionalLight(const Time& time, const KeyboardComponent& keyboard)
{
	const auto elapsedTime = time.elapsed.GetSeconds();

	// update light color intensity
	{
		const float modulationStepValue = elapsedTime * k_lightModulationRate;

		auto lightColor = m_directionalLight->GetColor();
		if (::utils::UpdateValue(lightColor.a, modulationStepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::Home, Key::End)))
		{
			m_directionalLight->SetColor(lightColor);
		}
	}

	// rotate directional light
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
		// test quaternion rotation
		const auto rotation = math::Quaternion::RotationPitchYawRoll(rotationAmount.y, rotationAmount.x, 0.f);
		m_directionalLight->Rotate(rotation);
	}
}

void DiffuseLightingDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	const auto &world = GetWorldMatrix();
	auto wvp = world;
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();

	m_material->GetWVP() << wvp;
	m_material->GetWorld() << world;
	m_material->GetAmbientColor() << m_ambientColor.ToVector4();

	m_material->GetLightData() << DirectionalLightData(*m_directionalLight);

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
