#include "EnvironmentMappingDemo.h"

#include <library/Components/KeyboardComponent.h>
#include <library/Components/CameraComponent.h>
#include <library/Components/TextComponent.h>

#include <library/Application.h>
#include <library/Path.h>
#include <library/Utils.h>
#include <library/Time.h>

#include <library/Effect/Effect.h>
#include <library/Effect/EffectPass.h>
#include <library/Effect/EffectTechnique.h>
#include <library/Effect/EffectVariable.h>

#include <sstream>

using namespace library;

namespace
{
	const auto k_environmentColor = Color::White;
}

EnvironmentMappingDemo::EnvironmentMappingDemo()
	: m_reflectionAmount(1.0f)
	, m_ambientColor(1.0f, 1.0f, 1.0f, 1.0f)
{
	SetModelName("Sphere");
	SetTextureName("Checkerboard");
}

EnvironmentMappingDemo::~EnvironmentMappingDemo() = default;

void EnvironmentMappingDemo::Initialize(const Application& app)
{
	InitializeMaterial(app, "EnvironmentMapping");
	SceneComponent::Initialize(app);

	m_environmentMapTexture = app.LoadTexture("Maskonaive2_1024");

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction(
		[this]() -> std::wstring
		{
			std::wostringstream woss;
			woss <<
				L"Reflection Amount (+Up/-Down): " << m_reflectionAmount << "\n";
			return woss.str();
		}
	);
	m_text->Initialize(app);
}

void EnvironmentMappingDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		if (m_keyboard->IsKeyDown(Key::Up) && m_reflectionAmount < 1.0f)
		{
			m_reflectionAmount += elapsedTime;
			m_reflectionAmount = math::Min(m_reflectionAmount, 1.0f);
		}
		if (m_keyboard->IsKeyDown(Key::Down) && m_reflectionAmount > 0.0f)
		{
			m_reflectionAmount -= elapsedTime;
			m_reflectionAmount = math::Max(m_reflectionAmount, 0.0f);
		}
	}

	m_text->Update(time);

	SceneComponent::Update(time);
}

void EnvironmentMappingDemo::Draw_SetData(const MeshData& meshData)
{
	auto wvp = GetWorldMatrix();
	if (!!m_camera)
	{
		wvp *= m_camera->GetViewProjectionMatrix();

		m_material->GetCameraPosition() << m_camera->GetPosition();
	}

	m_material->GetAmbientColor() << m_ambientColor;
	m_material->GetEnvironmentColor() << k_environmentColor;

	m_material->GetWVP() << wvp;
	m_material->GetWorld() << GetWorldMatrix();
	m_material->GetReflectionAmount() << m_reflectionAmount;

	m_material->GetColorTexture() << meshData.texture.Get();
	m_material->GetEnvironmentMap() << m_environmentMapTexture.Get();

	SceneComponent::Draw_SetData(meshData);
}
