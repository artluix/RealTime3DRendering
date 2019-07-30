#include "Components/MultiplePointLightsDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>
#include <library/Components/PointLightComponent.h>
#include <library/Components/ProxyModelComponent.h>
#include <library/Components/TextComponent.h>
#include <library/Components/FullScreenQuadComponent.h>

#include <library/RenderTargets/MultipleRenderTarget.h>

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

constexpr unsigned k_minLightsCount = 1;
constexpr unsigned k_maxLightsCount = 4;
} // namespace

MultiplePointLightsDemo::MultiplePointLightsDemo()
	: m_lightsCount(1)

	, m_specularPower(25.f)
	, m_specularColor(1.f, 1.f, 1.f, 1.f)
	, m_ambientColor(1.f, 1.f, 1.f, 0.f)

	, m_isDeferred(false)
{
}

void MultiplePointLightsDemo::InitializeInternal()
{
	assert(!!GetCamera());

	CreateMaterialWithEffect("MultiplePointLights");

	// load model
	{
		Model model(GetApp(), "Sphere", true);
		m_primitivesData = m_material->CreatePrimitivesData(GetApp().GetDevice(), model);
	}

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().LoadTexture("EarthAtDay");

	m_lightGlues.resize(k_maxLightsCount);
	for (auto& lightGlue : m_lightGlues)
	{
		lightGlue.light = std::make_unique<PointLightComponent>();
		lightGlue.light->SetRadius(500.f);

		lightGlue.model = std::make_unique<ProxyModelComponent>("PointLightProxy", 0.5f);
		lightGlue.model->SetCamera(*GetCamera());
	}

	// set position for lights
	m_lightGlues[0].light->SetPosition(math::Vector3(0.f, -10.f, 10.f));
	m_lightGlues[1].light->SetPosition(math::Vector3(0.f, 10.f, 10.f));
	m_lightGlues[2].light->SetPosition(math::Vector3(-10.f, 0.f, 0.f));
	m_lightGlues[3].light->SetPosition(math::Vector3(10.f, 0.f, 0.f));

	// initialize light models
	for (auto& lightGlue : m_lightGlues)
	{
		lightGlue.model->SetPosition(lightGlue.light->GetPosition());
		lightGlue.model->Initialize(GetApp());
	}

	m_text = std::make_unique<TextComponent>();
	m_text->SetPosition(math::Vector2(0.f, 100.f));
	m_text->SetTextUpdateFunction([this]() -> std::wstring {
		std::wostringstream woss;
		woss << L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n"
			 << L"Point Light Intensity (+Home/-End): " << m_lightColor.a << "\n"
			 << L"Specular Power (+Insert/-Delete): " << m_specularPower << "\n"
			 << L"Lights Count (+Right Alt/-Left Alt): " << m_lightsCount << "\n"
			 << L"Lighting type (Space): " << (m_isDeferred ? "Deferred" : "Forward") << "\n"
			 << L"Move Point Light (Numpad: 8/2, 4/6, 3/9)\n";
		return woss.str();
	});
	m_text->Initialize(GetApp());

	// deferred
	m_renderTarget = std::make_unique<MultipleRenderTarget>(GetApp(), 3); // Color + Normal + WorldPosition = 3

	m_fullScreenQuad = std::make_unique<FullScreenQuadComponent>();
	m_fullScreenQuad->SetMaterialUpdateFunction(
		[this]()
		{
			m_material->GetColorTexture() << m_renderTarget->GetOutputTexture(Target::Color);
			m_material->GetNormalTexture() << m_renderTarget->GetOutputTexture(Target::Normal);
			m_material->GetWorldPositionTexture() << m_renderTarget->GetOutputTexture(Target::WorldPosition);
		}
	);
	m_fullScreenQuad->Initialize(GetApp());
}

//-------------------------------------------------------------------------

void MultiplePointLightsDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		if (m_keyboard->WasKeyPressed(Key::Alt_Left) && m_lightsCount > k_minLightsCount)
			m_lightsCount--;
		if (m_keyboard->WasKeyPressed(Key::Alt_Right) && m_lightsCount < k_maxLightsCount)
			m_lightsCount++;

		if (m_keyboard->WasKeyPressed(Key::Space))
			m_isDeferred = !m_isDeferred;
	}

	UpdateAmbientLight(time);
	UpdatePointLight(time);
	UpdateSpecularLight(time);

	m_text->Update(time);

	if (m_isDeferred)
		m_fullScreenQuad->Update(time);

	for (unsigned i = 0; i < m_lightsCount; i++)
	{
		m_lightGlues[i].model->Update(time);
	}

	PrimitiveComponent::Update(time);
}

void MultiplePointLightsDemo::UpdateAmbientLight(const Time& time)
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

void MultiplePointLightsDemo::UpdatePointLight(const Time& time)
{
	static float pointLightIntensity = m_lightColor.a;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		// update light intensity
		if (m_keyboard->IsKeyDown(Key::Home) && pointLightIntensity < k_byteMax)
		{
			pointLightIntensity += k_lightModulationRate * elapsedTime;

			m_lightColor.a = math::Min(pointLightIntensity, k_byteMax);

			for (unsigned i = 0; i < m_lightsCount; i++)
			{
				m_lightGlues[i].light->SetColor(m_lightColor);
			}
		}

		if (m_keyboard->IsKeyDown(Key::End) && pointLightIntensity > 0)
		{
			pointLightIntensity -= k_lightModulationRate * elapsedTime;

			m_lightColor.a = math::Max(pointLightIntensity, 0.f);

			for (unsigned i = 0; i < m_lightsCount; i++)
			{
				m_lightGlues[i].light->SetColor(m_lightColor);
			}
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

			for (unsigned i = 0; i < m_lightsCount; i++)
			{
				auto& lightGlue = m_lightGlues[i];
				const auto position = lightGlue.light->GetPosition() + movement;

				lightGlue.light->SetPosition(position);
				lightGlue.model->SetPosition(position);
			}
		}
	}
}

void MultiplePointLightsDemo::UpdateSpecularLight(const Time& time)
{
	static float specularLightIntensity = m_specularPower;

	if (!!m_keyboard)
	{
		const auto elapsedTime = time.elapsed.GetSeconds();

		if (m_keyboard->IsKeyDown(Key::Insert) && specularLightIntensity < k_byteMax)
		{
			specularLightIntensity += k_lightModulationRate * elapsedTime;
			specularLightIntensity = math::Min(specularLightIntensity, k_byteMax);

			m_specularPower = specularLightIntensity;
		}

		if (m_keyboard->IsKeyDown(Key::Delete) && specularLightIntensity > 0)
		{
			specularLightIntensity -= k_lightModulationRate * elapsedTime;
			specularLightIntensity = math::Max(specularLightIntensity, 0.f);

			m_specularPower = specularLightIntensity;
		}
	}
}

//-------------------------------------------------------------------------

void MultiplePointLightsDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	if (m_isDeferred)
	{

	}
	else
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
		m_material->GetSpecularColor() << m_specularColor.ToVector4();
		m_material->GetAmbientColor() << m_ambientColor.ToVector4();
		m_material->GetModelTexture() << m_textures[Texture::Default].Get();

		std::vector<PointLight> pointLights;
		pointLights.resize(m_lightsCount);

		for (unsigned i = 0; i < m_lightsCount; i++)
		{
			pointLights[i].position = m_lightGlues[i].light->GetPosition();
			pointLights[i].lightRadius = m_lightGlues[i].light->GetRadius();
			pointLights[i].color = m_lightGlues[i].light->GetColor();
		}

		m_material->GetLightsCount() << m_lightsCount;
		m_material->GetPointLights() << pointLights;

		ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
	}
}
