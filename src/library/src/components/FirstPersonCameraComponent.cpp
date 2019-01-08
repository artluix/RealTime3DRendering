#include "StdAfx.h"
#include "library/components/FirstPersonCameraComponent.h"

#include "library/Application.h"
#include "library/Logger.h"
#include "library/Math.h"

#include "library/components/KeyboardComponent.h"
#include "library/components/MouseComponent.h"

namespace library
{
	namespace
	{
		constexpr float k_defaultMouseSensitivity = 0.1f;
		constexpr float k_defaultRotationRate = math::ToRadians(0.5f);
		constexpr float k_defaultMovementRate = 0.1f;
	}

	FirstPersonCameraComponent::FirstPersonCameraComponent(const Application& app)
		: CameraComponent(app)
		, m_rotationStartPoint(0.f, 0.f)
		, m_mouseSensitivity(k_defaultMouseSensitivity)
		, m_rotationRate(k_defaultRotationRate)
		, m_movementRate(k_defaultMovementRate)
	{
	}

	FirstPersonCameraComponent::FirstPersonCameraComponent(
		const Application& app,
		const float fieldOfView,
		const float aspectRatio,
		const float nearPlaneDistance,
		const float farPlaneDistance
	)
		: CameraComponent(app, fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance)
		, m_mouseSensitivity(k_defaultMouseSensitivity)
		, m_rotationRate(k_defaultRotationRate)
		, m_movementRate(k_defaultMovementRate)
		, m_rotationStartPoint(0.f, 0.f)
	{
	}

	void FirstPersonCameraComponent::SetKeyboard(const KeyboardComponent& keyboard)
	{
		if (m_keyboard != &keyboard)
		{
			m_keyboard = &keyboard;
		}
	}

	void FirstPersonCameraComponent::SetMouse(const MouseComponent& mouse)
	{
		if (m_mouse != &mouse)
		{
			m_mouse = &mouse;
		}
	}

	void FirstPersonCameraComponent::Initialize()
	{
		CameraComponent::Initialize();
	}

	void FirstPersonCameraComponent::Update(const Time& time)
	{
		const auto elapsedTime = time.elapsed.GetMilliseconds<float>();

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
				Logger::Info("Rotation delta: %s", rotationDelta.ToString().c_str());

				const auto rotation = rotationDelta * m_rotationRate * elapsedTime;

				const auto pitchMatrix = math::Matrix4::RotationAxis(m_right, rotation.y);
				const auto yawMatrix = math::Matrix4::RotationY(rotation.x);

				ApplyRotation(pitchMatrix * yawMatrix);
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
				auto position = m_position;

				const auto strafe = m_right * movement.x;
				position += strafe;

				const auto forward = m_direction * movement.y;
				position += forward;

				SetPosition(position);
			}
		}

		CameraComponent::Update(time);
	}

} // namespace library
