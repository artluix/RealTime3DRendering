#include "DisplacementMappingDemo.h"

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
constexpr float k_displacementRate = 5.f;
} // namespace

DisplacementMappingDemo::DisplacementMappingDemo()
	: m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)
	, m_displacementScale(0.f)
{}

DisplacementMappingDemo::~DisplacementMappingDemo() = default;

//-------------------------------------------------------------------------

void DisplacementMappingDemo::InitializeInternal()
{
	assert(!!GetCamera());

	CreateMaterialWithEffect("DisplacementMapping");

	// load model
	{
		Model model(GetApp(), "Plane", true);
		m_primitivesData = m_material->CreatePrimitivesData(GetApp().GetDevice(), model);
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().LoadTexture("Blocks_COLOR_RGB");
	m_textures[Texture::DisplacementMap] = GetApp().LoadTexture("Blocks_DISP");

	m_pointLight = std::make_unique<PointLightComponent>();
	m_pointLight->SetupProxyModel(*GetCamera());
	m_pointLight->SetRadius(50.f);
	m_pointLight->SetPosition(math::Vector3(0.f, 0.f, 10.f));

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		std::wostringstream woss;
		woss << L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n"
			 << L"Point Light Intensity (+Home/-End): " << m_pointLight->GetColor().a << "\n"
			 << L"Displacement Scale (+Insert/-Delete): " << m_displacementScale << "\n"
			 << L"Move Point Light (Numpad: 8/2, 4/6, 3/9)\n";
		return woss.str();
	});
	m_text->Initialize(GetApp());

	SetRotation(math::Vector3(math::Pi_Div_2, 0.f, 0.f));
}

void DisplacementMappingDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto& keyboard = *m_keyboard;

		UpdateAmbientLight(time, keyboard);
		UpdatePointLight(time, keyboard);
		UpdateDisplacement(time, keyboard);
	}

	m_text->Update(time);
	m_pointLight->Update(time);

	PrimitiveComponent::Update(time);
}

void DisplacementMappingDemo::UpdateAmbientLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_ambientColor.a, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::PageUp, Key::PageDown));
}

void DisplacementMappingDemo::UpdatePointLight(const Time& time, const KeyboardComponent& keyboard)
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

		m_pointLight->SetPosition(m_pointLight->GetPosition() + movement);
	}
}

void DisplacementMappingDemo::UpdateDisplacement(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_displacementRate;
	::utils::UpdateValue(m_displacementScale, stepValue, math::Interval(.0f, 2.f), keyboard, KeyPair(Key::Insert, Key::Delete));
}

void DisplacementMappingDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	const auto& world = GetWorldMatrix();
	auto wvp = world;

	if (auto camera = GetCamera())
	{
		wvp *= camera->GetViewProjectionMatrix();

		m_material->GetCameraPosition() << camera->GetPosition();
	}

	m_material->GetAmbientColor() << m_ambientColor.ToVector4();

	const auto isLightVisible = m_pointLight->IsVisibleFrom(GetPosition());
	m_material->GetPointLightsCount() << unsigned(isLightVisible);
	if (isLightVisible)
		m_material->GetPointLights() << PointLightData(*m_pointLight);

	m_material->GetWVP() << wvp;
	m_material->GetWorld() << world;
	m_material->GetSpecularPower() << m_specularPower;
	m_material->GetSpecularColor() << m_specularColor.ToVector4();
	m_material->GetDisplacementScale() << m_displacementScale;

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();
	m_material->GetDisplacementMap() << m_textures[Texture::DisplacementMap].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
