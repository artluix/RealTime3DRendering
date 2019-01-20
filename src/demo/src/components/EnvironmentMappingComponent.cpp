#include "components/EnvironmentMappingComponent.h"

#include <library/components/KeyboardComponent.h>
#include <library/components/CameraComponent.h>
#include <library/components/TextComponent.h>

#include <library/Path.h>
#include <library/Utils.h>
#include <library/Time.h>

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
		const auto k_environmentColor = Color::White;

		const auto k_effectPath = utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
			Path("../data/effects/EnvironmentMapping_d.fxc")
#else
			Path("../data/effects/EnvironmentMapping.fxc")
#endif
		);
		const auto k_modelPath = utils::GetExecutableDirectory().Join(Path("../data/models/Sphere.obj"));
		const auto k_colorTexturePath = utils::GetExecutableDirectory().Join(Path("../data/textures/Checkerboard.dds"));
		const auto k_environmentMapTexturePath = utils::GetExecutableDirectory().Join(Path("../data/textures/Maskonaive2_1024.dds"));
	}

	EnvironmentMappingComponent::EnvironmentMappingComponent(const Application& app)
		: SceneComponent(app)
		, InputReceivableComponent()
		, m_reflectionAmount(1.0f)
		, m_ambientColor(1.0f, 1.0f, 1.0f, 1.0f)
	{
		SetModelPath(k_modelPath);
		SetTexturePath(k_colorTexturePath);
	}

	EnvironmentMappingComponent::~EnvironmentMappingComponent() = default;

	void EnvironmentMappingComponent::Initialize()
	{
		m_effect = Effect::Create(m_app, k_effectPath);
		m_effect->LoadCompiled();

		m_material = std::make_unique<EnvironmentMappingMaterial>(*m_effect);
		m_material->Initialize();

		DrawableComponent::Initialize();
		LoadTexture(k_environmentMapTexturePath, m_environmentMapShaderResourceView);

		m_text = std::make_unique<TextComponent>(m_app);
		m_text->SetPosition(math::Vector2(0.f, 100.f));
		m_text->SetTextGeneratorFunction(
			[this]() -> std::wstring
			{
				std::wostringstream woss;
				woss <<
					L"Reflection Amount (+Up/-Down): " << m_reflectionAmount << "\n";
				return woss.str();
			}
		);
		m_text->Initialize();
	}

	void EnvironmentMappingComponent::Update(const Time& time)
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
	}

	void EnvironmentMappingComponent::SetEffectData()
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

		m_material->GetColorTexture() << m_textureShaderResourceView.Get();
		m_material->GetEnvironmentMap() << m_environmentMapShaderResourceView.Get();

		DrawableComponent::SetEffectData();
	}
} // namespace demo
