#include "components/DiffuseLightingEffectMaterialComponent.h"

#include <library/components/CameraComponent.h>
#include <library/components/KeyboardComponent.h>
#include <library/components/DirectionalLightComponent.h>
#include <library/components/ProxyModelComponent.h>
#include <library/components/TextComponent.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>
#include <library/Exception.h>

#include <library/effect/Effect.h>
#include <library/effect/EffectPass.h>
#include <library/effect/EffectTechnique.h>
#include <library/effect/EffectVariable.h>

#include <sstream>

namespace demo
{
	using namespace library;

	namespace
	{
		constexpr float k_byteMax = static_cast<float>(0xFF);

		constexpr float k_lightModulationRate = k_byteMax / 40;
		constexpr math::Vector2 k_lightRotationRate = math::Vector2(math::Pi_2);

		constexpr float k_proxyModelDistance = 10.f;
		constexpr auto k_rotationOffset = math::Vector3(0.f, math::Pi_Div_2, 0.f);

		const auto k_effectPath = utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
			Path("../data/effects/DiffuseLighting_d.fxc")
#else
			Path("../data/effects/DiffuseLighting.fxc")
#endif
		);
		const auto k_modelPath = utils::GetExecutableDirectory().Join(Path("../data/models/Sphere.obj"));
		const auto k_proxyModelPath = utils::GetExecutableDirectory().Join(Path("../data/models/DirectionalLightProxy.obj"));
		const auto k_texturePath = utils::GetExecutableDirectory().Join(Path("../data/textures/EarthComposite.dds"));
	}

	DiffuseLightingEffectMaterialComponent::DiffuseLightingEffectMaterialComponent(const Application& app)
		: SceneComponent(app)
		, InputReceivableComponent()
		, m_ambientColor(1.f, 1.f, 1.f, 0.f)
	{
		SetModelPath(k_modelPath);
		SetTexturePath(k_texturePath);
	}

	DiffuseLightingEffectMaterialComponent::~DiffuseLightingEffectMaterialComponent() = default;

	void DiffuseLightingEffectMaterialComponent::Initialize()
	{
		assert(!!m_camera);

		m_effect = Effect::Create(m_app, k_effectPath);
		m_effect->LoadCompiled();

		m_material = std::make_unique<DiffuseLightingEffectMaterial>(*m_effect);
		m_material->Initialize();

		DrawableComponent::Initialize();

		m_directionalLight = std::make_unique<DirectionalLightComponent>();

		m_proxyModel = std::make_unique<ProxyModelComponent>(m_app, k_proxyModelPath, 0.5f);
		m_proxyModel->Rotate(math::Vector3(k_rotationOffset));
		m_proxyModel->SetPosition(GetPosition() + -m_directionalLight->GetDirection() * k_proxyModelDistance);
		m_proxyModel->SetCamera(*m_camera);
		m_proxyModel->Initialize();

		m_text = std::make_unique<TextComponent>(m_app);
		m_text->SetPosition(math::Vector2(0.f, 100.f));
		m_text->SetTextGeneratorFunction(
			[this]() -> std::wstring
			{
				std::wostringstream woss;
				woss <<
					L"Ambient Intensity (+PageUp/-PageDown): " << m_ambientColor.a << "\n" <<
					L"Directional Light Intensity (+Home/-End): " << m_directionalLight->GetColor().a << "\n" <<
					L"Rotate Directional Light (Arrow Keys)\n";
				return woss.str();
			}
		);
		m_text->Initialize();
	}

	void DiffuseLightingEffectMaterialComponent::Update(const Time& time)
	{
		UpdateDirectionalLight(time);
		UpdateAmbientLight(time);

		m_text->Update(time);
		m_proxyModel->Update(time);
	}

	void DiffuseLightingEffectMaterialComponent::UpdateAmbientLight(const Time& time)
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

	void DiffuseLightingEffectMaterialComponent::UpdateDirectionalLight(const Time& time)
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

				const auto& dirLightRot = m_directionalLight->GetRotation();
				auto proxyModelRotation = math::Vector3(0.f, dirLightRot.y, dirLightRot.x) + k_rotationOffset;
				m_proxyModel->SetRotation(proxyModelRotation);
				m_proxyModel->SetPosition(GetPosition() + -m_directionalLight->GetDirection() * k_proxyModelDistance);
			}
		}
	}
	
	void DiffuseLightingEffectMaterialComponent::SetEffectData()
	{
		auto wvp = GetWorldMatrix();
		if (!!m_camera)
			wvp *= m_camera->GetViewProjectionMatrix();
		
		m_material->GetWVP() << wvp;
		m_material->GetWorld() << GetWorldMatrix();
		m_material->GetAmbientColor() << m_ambientColor;
		m_material->GetLightColor() << m_directionalLight->GetColor();
		m_material->GetLightDirection() << m_directionalLight->GetDirection();
		m_material->GetColorTexture() << m_textureShaderResourceView.Get();

		DrawableComponent::SetEffectData();
	}
} // namespace demo
