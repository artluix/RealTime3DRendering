#include "components/BasicEffectMaterialComponent.h"

#include <library/components/CameraComponent.h>
#include <library/components/KeyboardComponent.h>

#include <library/effect/Effect.h>
#include <library/effect/EffectVariable.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Exception.h>

namespace demo
{
	using namespace library;
	
	namespace
	{
		constexpr float k_rotationAngle = math::Pi_Div_2;
		constexpr float k_movementRate = 0.01f;

		const auto k_effectPath = utils::GetExecutableDirectory().Join(
#if defined(DEBUG) || defined(DEBUG)
			Path("../data/effects/Basic_d.fxc")
#else
			Path("../data/effects/Basic.fxc")
#endif
		);
		const auto k_modelPath = utils::GetExecutableDirectory().Join(Path("../data/models/Sphere.obj"));
	}

	BasicEffectMaterialComponent::BasicEffectMaterialComponent(const Application& app) 
		: SceneComponent(app)
		, InputReceivableComponent()
	{
		SetModelPath(k_modelPath);
	}

	void BasicEffectMaterialComponent::Initialize()
	{
		m_effect = Effect::Create(m_app, k_effectPath);
		m_effect->LoadCompiled();

		m_material = std::make_unique<BasicEffectMaterial>(*m_effect);
		m_material->Initialize();

		DrawableComponent::Initialize();
	}

	void BasicEffectMaterialComponent::Update(const Time& time)
	{
		if (!!m_keyboard)
		{
			// rotation
			if (m_keyboard->IsKeyDown(Key::R))
			{
				const auto rotationDelta = k_rotationAngle * time.elapsed.GetSeconds();
				Rotate(rotationDelta);
			}

			// movement
			{
				math::Vector2 movementAmount;
				if (m_keyboard->IsKeyDown(Key::Up))
				{
					movementAmount.y += 1.0f;
				}

				if (m_keyboard->IsKeyDown(Key::Down))
				{
					movementAmount.y -= 1.0f;
				}

				if (m_keyboard->IsKeyDown(Key::Left))
				{
					movementAmount.x -= 1.0f;
				}

				if (m_keyboard->IsKeyDown(Key::Right))
				{
					movementAmount.x += 1.0f;
				}

				if (movementAmount)
				{
					Translate(math::Vector3(movementAmount * k_movementRate));
				}
			}
		}
	}

	void BasicEffectMaterialComponent::SetEffectData()
	{
		auto wvp = GetWorldMatrix();
		if (!!m_camera)
			wvp *= m_camera->GetViewProjectionMatrix();

		m_material->GetWorldViewProjection() << wvp;

		DrawableComponent::SetEffectData();
	}
} // namespace demo
