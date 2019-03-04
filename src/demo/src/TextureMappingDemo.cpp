#include "TextureMappingDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>
#include <library/Exception.h>

#include <library/Effect/Effect.h>
#include <library/Effect/EffectTechnique.h>
#include <library/Effect/EffectVariable.h>

using namespace library;

namespace
{
	constexpr float k_rotationAngle = math::Pi_Div_2;
	constexpr float k_movementRate = 0.01f;
}

TextureMappingDemo::TextureMappingDemo()
{
	SetModelName("Sphere");
	SetTextureName("EarthComposite");
}

void TextureMappingDemo::Initialize(const Application& app)
{
	InitializeMaterial(app, "TextureMapping");
	SceneComponent::Initialize(app);
}

void TextureMappingDemo::Update(const Time& time)
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

	DrawableComponent::Update(time);
}

void TextureMappingDemo::Draw_SetData()
{
	auto wvp = GetWorldMatrix();
	if (!!m_camera)
		wvp *= m_camera->GetViewProjectionMatrix();
	m_material->GetWVP() << wvp;

	m_material->GetColorTexture() << GetTexture();

	SceneComponent::Draw_SetData();
}
