#include "TextureMappingDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Path.h>

#include <library/Math/Math.h>
#include <library/Model/Model.h>

#include <library/Effect/Effect.h>
#include <library/Effect/EffectTechnique.h>
#include <library/Effect/EffectVariable.h>

using namespace library;

namespace
{
constexpr float k_rotationAngle = math::Pi_Div_2;
constexpr float k_movementRate = 0.01f;
} // namespace

void TextureMappingDemo::InitializeInternal()
{
	InitializeMaterial("TextureMapping");

	Model model(GetApp(), "Sphere", true);
	m_primitivesData = m_material->CreatePrimitivesData(GetApp().GetDevice(), model);

	m_textures.resize(Texture::Count);
	m_textures[Texture::Default] = GetApp().LoadTexture("EarthComposite");
}

void TextureMappingDemo::Update(const Time& time)
{
	if (!!m_keyboard)
	{
		// rotation
		if (m_keyboard->IsKeyDown(Key::R))
		{
			const auto rotationDelta = k_rotationAngle * time.elapsed.GetSeconds();
			Rotate(math::Quaternion::RotationPitchYawRoll(math::Vector3(rotationDelta)));
		}

		// movement
		{
			math::Vector2i movementAmount;
			if (m_keyboard->IsKeyDown(Key::Up))
				movementAmount.y++;

			if (m_keyboard->IsKeyDown(Key::Down))
				movementAmount.y--;

			if (m_keyboard->IsKeyDown(Key::Left))
				movementAmount.x--;

			if (m_keyboard->IsKeyDown(Key::Right))
				movementAmount.x++;

			if (movementAmount)
				Translate(math::Vector3(movementAmount * k_movementRate));
		}
	}

	PrimitiveComponent::Update(time);
}

void TextureMappingDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();

	m_material->GetWVP() << wvp;
	m_material->GetColorTexture() << m_textures[Texture::Default].Get();

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
