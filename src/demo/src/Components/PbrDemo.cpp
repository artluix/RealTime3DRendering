#include "PbrDemo.h"

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
constexpr float k_lightModulationRate = 1.f;
constexpr float k_lightMovementRate = 10.f;
} // namespace

PbrDemo::PbrDemo()
	: m_ambientColor(1.f, 1.f, 1.f, 0.f)
	, m_albedo(colors::Red)
	, m_metallic(0.5f)
	, m_roughness(0.5f)
	, m_ao(1.0f)
{}

PbrDemo::~PbrDemo() = default;

//-------------------------------------------------------------------------

void PbrDemo::InitializeInternal()
{
	assert(!!GetCamera());

	CreateMaterialWithEffect("PbrLights");

	// load model
	{
		Model model(GetApp(), "Sphere", true);
		CreatePrimitivesData(model);
	}

	SetScaling(math::Vector3(0.3f));

	m_textures.resize(Texture::Count);
	m_textures[Texture::Albedo] = GetApp().CreateTexture2DSRV("rustediron2_albedo.png");
	m_textures[Texture::Metallic] = GetApp().CreateTexture2DSRV("rustediron2_metallic.png");
	m_textures[Texture::Roughness] = GetApp().CreateTexture2DSRV("rustediron2_roughness.png");

	m_pointLight = std::make_unique<PointLightComponent>();
	m_pointLight->SetupProxyModel(*GetCamera());
	m_pointLight->SetRadius(500.f);
	m_pointLight->SetColor(math::Color(300.f));
	m_pointLight->SetPosition(math::Vector3(0.f, 0.f, 10.f));

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		std::wostringstream woss;
		woss << L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n"
			<< L"Point Light Intensity (+Home/-End): " << m_pointLight->GetColor().a << "\n"
			<< L"Move Point Light (Numpad: 8/2, 4/6, 3/9)\n";

		if (m_isValuesUsed)
		{
			woss << L"Metallic  (+1/-2): " << m_metallic << "\n"
				<< L"Roughness  (+3/-4): " << m_roughness << "\n"
				<< L"Ambient Occlusion  (+5/-6): " << m_ao << "\n";
		}

		return woss.str();
	});
	m_text->Initialize(GetApp());
}

void PbrDemo::Update(const Time & time)
{
	if (!!m_keyboard)
	{
		const auto& keyboard = *m_keyboard;

		UpdateAmbientLight(time, keyboard);
		UpdatePointLight(time, keyboard);
		UpdatePbrParameters(time, keyboard);
	}

	m_text->Update(time);
	m_pointLight->Update(time);

	PrimitiveComponent::Update(time);
}

void PbrDemo::UpdateAmbientLight(const Time & time, const KeyboardComponent & keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_ambientColor.a, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::PageUp, Key::PageDown));
}

void PbrDemo::UpdatePointLight(const Time & time, const KeyboardComponent & keyboard)
{
	const auto elapsedTime = time.elapsed.GetSeconds();

	// update light color intensity
	{
		const float modulationStepValue = elapsedTime * k_lightModulationRate * 10;

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

void PbrDemo::UpdatePbrParameters(const Time & time, const KeyboardComponent & keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;

	if (keyboard.WasKeyPressed(Key::Left_Shift))
	{
		if (m_isValuesUsed)
		{
			m_isValuesUsed = false;
			m_material->SetCurrentTechnique("maps");
		}
		else
		{
			m_isValuesUsed = true;
			m_material->SetCurrentTechnique("values");
		}
	}

	if (m_isValuesUsed)
	{
		::utils::UpdateValue(m_metallic, stepValue, math::UnitInterval, keyboard, KeyPair(Key::_1, Key::_2));
		::utils::UpdateValue(m_roughness, stepValue, math::UnitInterval, keyboard, KeyPair(Key::_3, Key::_4));
	}

	::utils::UpdateValue(m_ao, stepValue, math::UnitInterval, keyboard, KeyPair(Key::_5, Key::_6));
}

void PbrDemo::Draw_SetData(const PrimitiveData & primitiveData)
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

	m_material->GetAmbientColor() << m_ambientColor.ToVector();

	if (m_isValuesUsed)
	{
		m_material->GetAlbedo() << m_albedo.ToVector().xyz;
		m_material->GetMetallic() << m_metallic;
		m_material->GetRoughness() << m_roughness;
		m_material->GetAO() << m_ao;
	}
	else
	{
		m_material->GetAlbedoMap() << m_textures[Texture::Albedo].Get();
		m_material->GetMetallicMap() << m_textures[Texture::Metallic].Get();
		m_material->GetRoughnessMap() << m_textures[Texture::Roughness].Get();
		//m_material->GetAOMap() << m_textures[Texture::AO].Get();
		m_material->GetAO() << m_ao;
	}

	const auto isLightVisible = m_pointLight->IsVisibleFrom(GetPosition());
	m_material->GetPointLightsCount() << unsigned(isLightVisible);
	if (isLightVisible)
		m_material->GetPointLights() << PointLightData(*m_pointLight);

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
