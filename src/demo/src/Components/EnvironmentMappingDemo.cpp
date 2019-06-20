#include "EnvironmentMappingDemo.h"

#include <library/Components/KeyboardComponent.h>
#include <library/Components/CameraComponent.h>
#include <library/Components/TextComponent.h>

#include <library/Application.h>
#include <library/Path.h>
#include <library/Utils.h>
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
constexpr auto k_environmentColor = colors::White;
}

EnvironmentMappingDemo::EnvironmentMappingDemo()
	: m_reflectionAmount(1.0f)
	, m_ambientColor(1.0f, 1.0f, 1.0f, 1.0f)
{}

EnvironmentMappingDemo::~EnvironmentMappingDemo() = default;

//-------------------------------------------------------------------------

void EnvironmentMappingDemo::InitializeInternal()
{
	CreateMaterialWithEffect("EnvironmentMapping");

	// load model
	{
		Model model(GetApp(), "Sphere", true);
		m_primitivesData = m_material->CreatePrimitivesData(GetApp().GetDevice(), model);
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().LoadTexture("Checkerboard");
	m_textures[Texture::EnvironmentMap] = GetApp().LoadTexture("Maskonaive2_1024");

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		std::wostringstream woss;
		woss << L"Reflection Amount (+Up/-Down): " << m_reflectionAmount << "\n";
		return woss.str();
	});
	m_text->Initialize(GetApp());
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

	PrimitiveComponent::Update(time);
}

void EnvironmentMappingDemo::Draw_SetData(const PrimitiveData& primitiveData)
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
	m_material->GetReflectionAmount() << m_reflectionAmount;

	m_material->GetAmbientColor() << m_ambientColor.ToVector4();
	m_material->GetEnvironmentColor() << k_environmentColor.ToVector4();

	m_material->GetColorTexture() << m_textures[Texture::Default].Get();
	m_material->GetEnvironmentMap() << m_textures[Texture::EnvironmentMap].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
