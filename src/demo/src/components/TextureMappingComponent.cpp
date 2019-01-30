#include "components/TextureMappingComponent.h"

#include <library/components/CameraComponent.h>
#include <library/components/KeyboardComponent.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>
#include <library/Exception.h>

#include <library/effect/Effect.h>
#include <library/effect/EffectTechnique.h>
#include <library/effect/EffectVariable.h>

namespace demo
{
	using namespace library;

	namespace
	{
		constexpr float k_rotationAngle = math::Pi_Div_2;
		constexpr float k_movementRate = 0.01f;
	}

	TextureMappingComponent::TextureMappingComponent()
	{
		SetModelName("Sphere");
		SetTextureName("EarthComposite");
	}

	void TextureMappingComponent::Initialize(const Application& app)
	{
		InitializeMaterial(app, "TextureMapping");
		MaterialComponent::Initialize(app);
	}

	void TextureMappingComponent::Update(const Time& time)
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

	void TextureMappingComponent::SetEffectData()
	{
		auto wvp = GetWorldMatrix();
		if (auto camera = GetCamera())
			wvp *= camera->GetViewProjectionMatrix();
		m_material->GetWVP() << wvp;

		m_material->GetColorTexture() << m_textureShaderResourceView.Get();

		MaterialComponent::SetEffectData();
	}
} // namespace demo
