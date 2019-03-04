#include "NormalMappingDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>
#include <library/Components/DirectionalLightComponent.h>
#include <library/Components/ProxyModelComponent.h>
#include <library/Components/TextComponent.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>
#include <library/Exception.h>

#include <library/Effect/Effect.h>
#include <library/Effect/EffectPass.h>
#include <library/Effect/EffectTechnique.h>
#include <library/Effect/EffectVariable.h>

#include <sstream>
#include <array>

using namespace library;

namespace
{
	constexpr float k_byteMax = static_cast<float>(0xFF);

	constexpr float k_lightModulationRate = 10.f;
	constexpr auto k_lightRotationRate = math::Vector2(math::Pi_2);

	constexpr auto k_proxyModelRotationOffset = math::Vector3(0.f, math::Pi_Div_2, 0.f);
	constexpr float k_proxyModelDistanceOffset = 3.f;
}

NormalMappingDemo::NormalMappingDemo()
	: m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)
{
	SetTextureName("Blocks_COLOR_RGB");
}

NormalMappingDemo::~NormalMappingDemo() = default;

void NormalMappingDemo::Initialize(const Application& app)
{
	assert(!!GetCamera());

	// build vertices manually
	{
		using Vertex = NormalMappingMaterial::Vertex;
		using DirectX::XMFLOAT4;
		using DirectX::XMFLOAT2;

		const auto right = DirectX::XMFLOAT3(math::Vector3::Right);
		const auto backward = DirectX::XMFLOAT3(math::Vector3::Backward);

		const std::array<Vertex, 6> vertices =
		{
			Vertex(XMFLOAT4(-0.5f, -0.5f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), backward, right),
			Vertex(XMFLOAT4(-0.5f, 0.5f, 0.0f, 1.0f), XMFLOAT2(0.0f, 0.0f), backward, right),
			Vertex(XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), backward, right),

			Vertex(XMFLOAT4(-0.5f, -0.5f, 0.0f, 1.0f), XMFLOAT2(0.0f, 1.0f), backward, right),
			Vertex(XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f), backward, right),
			Vertex(XMFLOAT4(0.5f, -0.5f, 0.0f, 1.0f), XMFLOAT2(1.0f, 1.0f), backward, right),
		};

		m_input.vertices.count = vertices.size();
		m_input.vertices.buffer = library::Material::CreateVertexBuffer(
			app.GetDevice(),
			vertices
		);
	}

	InitializeMaterial(app, "NormalMapping");
	SceneComponent::Initialize(app);

	m_normalMapTexture = app.LoadTexture("Blocks_NORM");

	m_directionalLight = std::make_unique<DirectionalLightComponent>();

	m_proxyModel = std::make_unique<ProxyModelComponent>("DirectionalLightProxy", 0.2f);
	m_proxyModel->SetPosition(
		GetPosition() + -m_directionalLight->GetDirection() * k_proxyModelDistanceOffset
	);
	m_proxyModel->SetRotation(GetRotation() + k_proxyModelRotationOffset);
	m_proxyModel->SetCamera(*GetCamera());
	m_proxyModel->Initialize(app);

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss <<
				L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n" <<
				L"Directional Light Intensity (+Home/-End): " << m_directionalLight->GetColor().a << "\n" <<
				L"Specular Power (+Insert/-Delete): " << m_specularPower << "\n" <<
				L"Rotate Directional Light (Arrow Keys)\n";
			return woss.str();
		}
	);
	m_text->Initialize(app);
}

void NormalMappingDemo::Update(const Time& time)
{
	UpdateAmbientLight(time);
	UpdateDirectionalLight(time);
	UpdateSpecularLight(time);

	m_text->Update(time);
	m_proxyModel->Update(time);

	SceneComponent::Update(time);
}

void NormalMappingDemo::UpdateAmbientLight(const Time& time)
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

void NormalMappingDemo::UpdateDirectionalLight(const Time& time)
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
		}

		if (m_keyboard->IsKeyDown(Key::End) && directionalLightIntensity > 0)
		{
			directionalLightIntensity -= k_lightModulationRate * elapsedTime;

			auto directionalLightColor = m_directionalLight->GetColor();
			directionalLightColor.a = math::Max(directionalLightIntensity, 0.f);
			m_directionalLight->SetColor(directionalLightColor);
		}

		// rotate directional light
		math::Vector2 rotationAmount;
		if (m_keyboard->IsKeyDown(Key::Left))
			rotationAmount.x -= k_lightRotationRate.x * elapsedTime;

		if (m_keyboard->IsKeyDown(Key::Right))
			rotationAmount.x += k_lightRotationRate.x * elapsedTime;

		if (m_keyboard->IsKeyDown(Key::Up))
			rotationAmount.y -= k_lightRotationRate.y * elapsedTime;

		if (m_keyboard->IsKeyDown(Key::Down))
			rotationAmount.y += k_lightRotationRate.y * elapsedTime;

		if (rotationAmount)
		{
			m_directionalLight->Rotate(math::Vector3(rotationAmount.y, rotationAmount.x, 0.f));

			const auto dirLightRot = m_directionalLight->GetRotation();

			m_proxyModel->SetPosition(
				GetPosition() + -m_directionalLight->GetDirection() * k_proxyModelDistanceOffset
			);
			m_proxyModel->SetRotation(
				math::Vector3(dirLightRot.z, dirLightRot.y, dirLightRot.x) + k_proxyModelRotationOffset
			);
		}
	}
}

void NormalMappingDemo::UpdateSpecularLight(const Time& time)
{
	static float specularLightIntensity = m_specularPower;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		if (m_keyboard->IsKeyDown(Key::Insert) && specularLightIntensity < k_byteMax)
		{
			specularLightIntensity += k_lightModulationRate * elapsedTime;
			specularLightIntensity = math::Min(specularLightIntensity, k_byteMax);

			m_specularPower = specularLightIntensity;;
		}

		if (m_keyboard->IsKeyDown(Key::Delete) && specularLightIntensity > 0)
		{
			specularLightIntensity -= k_lightModulationRate * elapsedTime;
			specularLightIntensity = math::Max(specularLightIntensity, 0.f);

			m_specularPower = specularLightIntensity;
		}
	}
}

void NormalMappingDemo::Draw_SetData()
{
	auto wvp = GetWorldMatrix();
	if (!!m_camera)
	{
		wvp *= m_camera->GetViewProjectionMatrix();

		m_material->GetCameraPosition() << m_camera->GetPosition();
	}

	m_material->GetWVP() << wvp;
	m_material->GetWorld() << GetWorldMatrix();
	m_material->GetSpecularPower() << m_specularPower;
	m_material->GetSpecularColor() << m_specularColor;
	m_material->GetAmbientColor() << m_ambientColor;
	m_material->GetLightColor() << m_directionalLight->GetColor();
	m_material->GetLightDirection() << m_directionalLight->GetDirection();
	m_material->GetColorTexture() << GetTexture();
	m_material->GetNormalMap() << m_normalMapTexture.Get();

	SceneComponent::Draw_SetData();
}
