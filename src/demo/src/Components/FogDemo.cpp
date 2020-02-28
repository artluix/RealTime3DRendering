#include "FogDemo.h"

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
		m_primitivesData = m_material->CreatePrimitivesData(GetApp().GetDevice(), model);
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().LoadTexture("EarthComposite");

	m_directionalLight = std::make_unique<DirectionalLightComponent>();

	m_proxyModel = std::make_unique<ProxyModelComponent>("DirectionalLightProxy", 0.2f);
	m_proxyModel->SetPosition(GetPosition() + math::Vector3(5.f));
	m_proxyModel->SetInitialTransform(math::Matrix4::RotationPitchYawRoll(k_proxyModelRotationOffset));
	m_proxyModel->SetCamera(*GetCamera());
	m_proxyModel->Initialize(GetApp());

	m_lightsData.dirLights[0] = m_directionalLight->GetData();
	m_lightsData.dirLightsCount = 1;

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
	if (!!m_keyboard && m_keyboard->WasKeyReleased(Key::Space))
	{
		m_fogEnabled = !m_fogEnabled;
		SetActiveTechnique();
	}

	UpdateDirectionalLight(time);
	UpdateAmbientLight(time);

	m_text->Update(time);
	m_proxyModel->Update(time);

	PrimitiveComponent::Update(time);
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

			m_lightsData.dirLights[0].color = directionalLightColor;
		}

		if (m_keyboard->IsKeyDown(Key::End) && directionalLightIntensity > 0)
		{
			directionalLightIntensity -= k_lightModulationRate * elapsedTime;

			auto directionalLightColor = m_directionalLight->GetColor();
			directionalLightColor.a = math::Max(directionalLightIntensity, 0.f);
			m_directionalLight->SetColor(directionalLightColor);

			m_lightsData.dirLights[0].color = directionalLightColor;
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
			m_proxyModel->Rotate(rotation);

			m_lightsData.dirLights[0] = m_directionalLight->GetColor();
		}
	}
}

void FogDemo::SetActiveTechnique()
{
	static const std::array<std::string, 2> techniques = {"fogDisabled", "fogEnabled"};
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

	m_material->GetLightsData() << m_lightsData;

	m_material->GetFogColor() << k_fogColor.ToVector4();
	m_material->GetFogStart() << m_fogStart;
	m_material->GetFogRange() << m_fogRange;

	m_material->GetWVP() << wvp;
	m_material->GetWorld() << world;

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
