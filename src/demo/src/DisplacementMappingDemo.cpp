#include "DisplacementMappingDemo.h"

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
constexpr auto k_proxyModelRotationOffset = math::Vector3(0.f, math::Pi_Div_2, 0.f);
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

	InitializeMaterial("DisplacementMapping");

	// load model
	{
		Model model(GetApp(), "Plane", true);
		m_primitivesData = GetMaterial()->CreatePrimitivesData(GetApp().GetDevice(), model);
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().LoadTexture("Blocks_COLOR_RGB");
	m_textures[Texture::DisplacementMap] = GetApp().LoadTexture("Blocks_DISP");

	m_pointLight = std::make_unique<PointLightComponent>();
	m_pointLight->SetRadius(50.f);
	m_pointLight->SetPosition(math::Vector3(0.f, 0.f, 10.f));

	m_proxyModel = std::make_unique<ProxyModelComponent>("PointLightProxy", 0.2f);
	m_proxyModel->SetPosition(m_pointLight->GetPosition());
	m_proxyModel->SetInitialTransform(math::Matrix4::RotationPitchYawRoll(k_proxyModelRotationOffset));
	m_proxyModel->SetCamera(*GetCamera());
	m_proxyModel->Initialize(GetApp());

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
	UpdateAmbientLight(time);
	UpdatePointLight(time);

	UpdateDisplacement(time);

	m_text->Update(time);
	m_proxyModel->Update(time);

	PrimitiveComponent::Update(time);
}

void DisplacementMappingDemo::UpdateAmbientLight(const Time& time)
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

void DisplacementMappingDemo::UpdatePointLight(const Time& time)
{
	static float pointLightIntensity = m_pointLight->GetColor().a;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		// update directional light intensity
		if (m_keyboard->IsKeyDown(Key::Home) && pointLightIntensity < k_byteMax)
		{
			pointLightIntensity += k_lightModulationRate * elapsedTime;

			auto pointLightColor = m_pointLight->GetColor();
			pointLightColor.a = math::Min(pointLightIntensity, k_byteMax);
			m_pointLight->SetColor(pointLightColor);
		}

		if (m_keyboard->IsKeyDown(Key::End) && pointLightIntensity > 0)
		{
			pointLightIntensity -= k_lightModulationRate * elapsedTime;

			auto pointLightColor = m_pointLight->GetColor();
			pointLightColor.a = math::Max(pointLightIntensity, 0.f);
			m_pointLight->SetColor(pointLightColor);
		}

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
			const auto movement = movementAmount * k_lightMovementRate * elapsedTime;

			m_pointLight->SetPosition(m_pointLight->GetPosition() + movement);
			m_proxyModel->SetPosition(m_pointLight->GetPosition());
		}
	}
}

void DisplacementMappingDemo::UpdateDisplacement(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		if (m_keyboard->IsKeyDown(Key::Insert) && m_displacementScale < 2.0f)
		{
			m_displacementScale += k_displacementRate * elapsedTime;
			m_displacementScale = math::Min(m_displacementScale, 2.0f);
		}

		if (m_keyboard->IsKeyDown(Key::Delete) && m_displacementScale > 0)
		{
			m_displacementScale -= k_displacementRate * elapsedTime;
			m_displacementScale = math::Max(m_displacementScale, 0.f);
		}
	}
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
	m_material->GetLightColor() << m_pointLight->GetColor().ToVector4();
	m_material->GetLightPosition() << m_pointLight->GetPosition();
	m_material->GetLightRadius() << m_pointLight->GetRadius();

	m_material->GetWVP() << wvp;
	m_material->GetWorld() << world;
	m_material->GetSpecularPower() << m_specularPower;
	m_material->GetSpecularColor() << m_specularColor.ToVector4();
	m_material->GetDisplacementScale() << m_displacementScale;

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();
	m_material->GetDisplacementMap() << m_textures[Texture::DisplacementMap].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
