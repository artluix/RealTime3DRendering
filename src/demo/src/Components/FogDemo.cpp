#include "FogDemo.h"

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
constexpr auto k_fogColor = colors::CornFlower;

constexpr float k_byteMax = static_cast<float>(0xFF);

constexpr float k_lightModulationRate = k_byteMax / 40;
constexpr auto k_lightRotationRate = math::Vector2(math::Pi_2);

constexpr auto k_proxyModelRotationOffset = math::Vector3(0.f, math::Pi_Div_2, 0.f);
} // namespace

//-------------------------------------------------------------------------

FogDemo::FogDemo()
	: m_ambientColor(1.f, 1.f, 1.f, 0.f)
	, m_fogStart(15.0f)
	, m_fogRange(20.0f)
	, m_fogEnabled(true)
{}

FogDemo::~FogDemo() = default;

//-------------------------------------------------------------------------

void FogDemo::InitializeInternal()
{
	assert(!!GetCamera());

	CreateMaterialWithEffect("Fog");

	// load model
	{
		Model model(GetApp(), "Sphere", true);
		CreatePrimitivesData(model);
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
			 << L"Rotate Directional Light (Arrow Keys)\n"
			 << L"Fog enabled (Space): " << (m_fogEnabled ? "Yes" : "No") << "\n"
			 << L"Fog start: " << m_fogStart << "\n"
			 << L"Fog range: " << m_fogRange << "\n";
		return woss.str();
	});
	m_text->Initialize(GetApp());

	SetActiveTechnique();
}

void FogDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto& keyboard = *m_keyboard;

		if (keyboard.WasKeyReleased(Key::Space))
		{
			m_fogEnabled = !m_fogEnabled;
			SetActiveTechnique();
		}

		UpdateDirectionalLight(time, keyboard);
		UpdateAmbientLight(time, keyboard);
	}

	m_text->Update(time);
	m_directionalLight->Update(time);

	PrimitiveComponent::Update(time);
}

void FogDemo::UpdateAmbientLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_ambientColor.a, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::PageUp, Key::PageDown));
}

void FogDemo::UpdateDirectionalLight(const Time& time, const KeyboardComponent& keyboard)
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
		// test quaternion rotation
		const auto rotation = math::Quaternion::RotationPitchYawRoll(rotationAmount.y, rotationAmount.x, 0.f);
		m_directionalLight->Rotate(rotation);
	}
}

void FogDemo::SetActiveTechnique()
{
	static const Array<std::string, 2> techniques = {"fogDisabled", "fogEnabled"};
	const auto& techniqueName = techniques[unsigned(m_fogEnabled)];

	m_material->SetCurrentTechnique(techniqueName);
}

void FogDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	const auto& world = GetWorldMatrix();
	auto wvp = world;

	if (auto camera = GetCamera())
	{
		wvp *= camera->GetViewProjectionMatrix();

		m_material->GetCameraPosition() << camera->GetPosition();
	}

	m_material->GetAmbientColor() << m_ambientColor.ToVector4();

	m_material->GetDirLights() << DirectionalLightData(*m_directionalLight);
	m_material->GetDirLightsCount() << 1;

	m_material->GetFogColor() << k_fogColor.ToVector4();
	m_material->GetFogStart() << m_fogStart;
	m_material->GetFogRange() << m_fogRange;

	m_material->GetWVP() << wvp;
	m_material->GetWorld() << world;

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
