#include "NormalMappingDemo.h"

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

constexpr float k_lightModulationRate = 10.f;
constexpr auto k_lightRotationRate = math::Vector2(math::Pi_2);

constexpr auto k_proxyModelRotationOffset = math::Vector3(0.f, math::Pi_Div_2, 0.f);
} // namespace

//-------------------------------------------------------------------------

NormalMappingDemo::NormalMappingDemo()
	: m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)
{}

NormalMappingDemo::~NormalMappingDemo() = default;

//-------------------------------------------------------------------------

void NormalMappingDemo::InitializeInternal()
{
	assert(!!GetCamera());

	CreateMaterialWithEffect("NormalMapping", false);
	//CreateMaterialWithEffect("NormalMapping", true);

	// build vertices manually
	{
		using Vertex = NormalMappingMaterial::Vertex;
		using namespace library::math;

		constexpr auto vertices = MakeArray(
			Vertex(Vector4(-0.5f, -0.5f, 0.0f, 1.0f), Vector2(0.0f, 1.0f), Direction::Backward, Direction::Right),
			Vertex(Vector4(-0.5f, 0.5f, 0.0f, 1.0f), Vector2(0.0f, 0.0f), Direction::Backward, Direction::Right),
			Vertex(Vector4(0.5f, 0.5f, 0.0f, 1.0f), Vector2(1.0f, 0.0f), Direction::Backward, Direction::Right),

			Vertex(Vector4(-0.5f, -0.5f, 0.0f, 1.0f), Vector2(0.0f, 1.0f), Direction::Backward, Direction::Right),
			Vertex(Vector4(0.5f, 0.5f, 0.0f, 1.0f), Vector2(1.0f, 0.0f), Direction::Backward, Direction::Right),
			Vertex(Vector4(0.5f, -0.5f, 0.0f, 1.0f), Vector2(1.0f, 1.0f), Direction::Backward, Direction::Right)
		);

		m_primitivesData.clear();
		auto& pd = m_primitivesData.emplace_back(PrimitiveData{});

		pd.vertexBuffer = VertexBufferData(GetApp().GetDevice(), MakeArrayBuffer(vertices));
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().CreateTexture2DSRV("Blocks_COLOR_RGB.tga");
	m_textures[Texture::NormalMap] = GetApp().CreateTexture2DSRV("Blocks_NORM.tga");

	m_directionalLight = std::make_unique<DirectionalLightComponent>();
	m_directionalLight->SetupProxyModel(*GetCamera());
	m_directionalLight->GetProxyModel()->SetPosition(GetPosition() + math::Vector3(2.f));

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		std::wostringstream woss;
		woss << L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n"
			 << L"Directional Light Intensity (+Home/-End): " << m_directionalLight->GetColor().a << "\n"
			 << L"Specular Power (+Insert/-Delete): " << m_specularPower << "\n"
			 << L"Rotate Directional Light (Arrow Keys)\n";
		return woss.str();
	});
	m_text->Initialize(GetApp());
}

void NormalMappingDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto& keyboard = *m_keyboard;

		UpdateAmbientLight(time, keyboard);
		UpdateDirectionalLight(time, keyboard);
		UpdateSpecularLight(time, keyboard);
	}

	m_text->Update(time);
	m_directionalLight->Update(time);

	PrimitiveComponent::Update(time);
}

void NormalMappingDemo::UpdateAmbientLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_ambientColor.a, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::PageUp, Key::PageDown));
}

void NormalMappingDemo::UpdateDirectionalLight(const Time& time, const KeyboardComponent& keyboard)
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

void NormalMappingDemo::UpdateSpecularLight(const Time& time, const KeyboardComponent& keyboard)
{
	const float stepValue = time.elapsed.GetSeconds() * k_lightModulationRate;
	::utils::UpdateValue(m_specularPower, stepValue, math::Interval(.0f, k_byteMax), keyboard, KeyPair(Key::Insert, Key::Delete));
}

//-------------------------------------------------------------------------

void NormalMappingDemo::Draw_SetData(const PrimitiveData& primitiveData)
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

	m_material->GetDirLights() << DirectionalLightData(*m_directionalLight);
	m_material->GetDirLightsCount() << 1;

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();
	m_material->GetNormalMap() << m_textures[Texture::NormalMap].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
