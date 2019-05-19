#include "StdAfx.h"
#include "library/Components/FirstPersonCameraComponent.h"

#include "library/Application.h"
#include "library/Logger.h"
#include "library/Math/Math.h"

#include "library/Components/KeyboardComponent.h"
#include "library/Components/MouseComponent.h"

namespace library
{

namespace
{
constexpr float k_defaultMouseSensitivity = 0.1f;
constexpr float k_defaultRotationRate = math::ToRadians(0.5f);
constexpr float k_defaultMovementRate = 0.02f;
} // namespace

//-------------------------------------------------------------------------

FirstPersonCameraComponent::FirstPersonCameraComponent()
	: m_rotationStartPoint(0.f, 0.f)
	, m_mouseSensitivity(k_defaultMouseSensitivity)
	, m_rotationRate(k_defaultRotationRate)
	, m_movementRate(k_defaultMovementRate)
{}

FirstPersonCameraComponent::FirstPersonCameraComponent(
	const float fieldOfView,
	const float aspectRatio,
	const float nearPlaneDistance,
	const float farPlaneDistance)
	: CameraComponent(fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance)
	, m_mouseSensitivity(k_defaultMouseSensitivity)
	, m_rotationRate(k_defaultRotationRate)
	, m_movementRate(k_defaultMovementRate)
	, m_rotationStartPoint(0.f, 0.f)
{}

FirstPersonCameraComponent::~FirstPersonCameraComponent() = default;

//-------------------------------------------------------------------------

void FirstPersonCameraComponent::Update(const Time& time)
{
	const auto elapsedTime = time.elapsed.GetMilliseconds();

	// compute rotation
	if (!!m_mouse)
	{
		if (m_mouse->WasButtonReleased(MouseButton::Left))
		{
			m_rotationStartPoint.x = 0.f;
			m_rotationStartPoint.y = 0.f;
		}

		if (m_mouse->WasButtonPressed(MouseButton::Left))
		{
			m_rotationStartPoint.x = m_mouse->GetX() * m_mouseSensitivity;
			m_rotationStartPoint.y = m_mouse->GetY() * m_mouseSensitivity;
		}

		if (m_mouse->IsButtonHeldDown(MouseButton::Left))
		{
			auto prevPoint = m_rotationStartPoint;

			m_rotationStartPoint.x = m_mouse->GetX() * m_mouseSensitivity;
			m_rotationStartPoint.y = m_mouse->GetY() * m_mouseSensitivity;

			const auto rotationDelta = m_rotationStartPoint - prevPoint;
			const auto rotation = rotationDelta * m_rotationRate * elapsedTime;

			const auto pitchQuat = math::Quaternion::RotationAxis(GetRight(), rotation.y);
			const auto yawQuat = math::Quaternion::RotationAxis(math::Vector3::Up, rotation.x); // y

			Rotate(pitchQuat * yawQuat);
		}
	}

	// compute movement
	if (!!m_keyboard)
	{
		math::Vector2 movementAmount;

		if (m_keyboard->IsKeyDown(Key::W))
		{
			movementAmount.y += 1.0f;
		}

		if (m_keyboard->IsKeyDown(Key::S))
		{
			movementAmount.y -= 1.0f;
		}

		if (m_keyboard->IsKeyDown(Key::A))
		{
			movementAmount.x -= 1.0f;
		}

		if (m_keyboard->IsKeyDown(Key::D))
		{
			movementAmount.x += 1.0f;
		}

		if (movementAmount)
		{
			const auto movement = movementAmount * m_movementRate * elapsedTime;
			auto position = GetPosition();

			const auto strafe = GetRight() * movement.x;
			position += strafe;

			const auto forward = GetDirection() * movement.y;
			position += forward;

			SetPosition(position);
		}
	}

	CameraComponent::Update(time);
}
} // namespace library
