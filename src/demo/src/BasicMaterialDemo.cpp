#include "BasicMaterialDemo.h"

#include <library/Components/CameraComponent.h>
#include <library/Components/KeyboardComponent.h>

#include <library/Effect/Effect.h>
#include <library/Effect/EffectVariable.h>

#include <library/Model/Model.h>

#include <library/Application.h>
#include <library/Utils.h>
#include <library/Math/Math.h>

using namespace library;

namespace
{
constexpr float k_rotationAngle = math::Pi_Div_2;
constexpr float k_movementRate = 0.01f;
} // namespace

void BasicMaterialDemo::InitializeInternal()
{
	InitializeMaterial("Basic");

	Model model(GetApp(), "Sphere", true);
	m_primitivesData = GetMaterial()->CreatePrimitivesData(GetApp().GetDevice(), model);
}

void BasicMaterialDemo::Update(const Time& time)
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
			math::Vector2 movementAmount;
			if (m_keyboard->IsKeyDown(Key::Up))
				movementAmount.y += 1.0f;

			if (m_keyboard->IsKeyDown(Key::Down))
				movementAmount.y -= 1.0f;

			if (m_keyboard->IsKeyDown(Key::Left))
				movementAmount.x -= 1.0f;

			if (m_keyboard->IsKeyDown(Key::Right))
				movementAmount.x += 1.0f;

			if (movementAmount)
				Translate(math::Vector3(movementAmount * k_movementRate));
		}
	}

	PrimitiveComponent::Update(time);
}

void BasicMaterialDemo::Draw_SetData(const PrimitiveData& primitiveData)
{
	auto wvp = GetWorldMatrix();
	if (auto camera = GetCamera())
		wvp *= camera->GetViewProjectionMatrix();

	m_material->GetWorldViewProjection() << math::XMMatrix(wvp);

	ConcreteMaterialPrimitiveComponent::Draw_SetData(primitiveData);
}
