#include "library/components/FirstPersonCameraComponent.h"

#include "library/Application.h"
#include "library/Logger.h"
#include "library/Math.h"

#include "library/components/KeyboardComponent.h"
#include "library/components/MouseComponent.h"

namespace library
{
	namespace components
	{

		const float FirstPersonCameraComponent::k_defaultMouseSensitivity = 0.1f;
		const float FirstPersonCameraComponent::k_defaultRotationRate = DirectX::XMConvertToRadians(0.5f);
		const float FirstPersonCameraComponent::k_defaultMovementRate = 0.1f;

		FirstPersonCameraComponent::FirstPersonCameraComponent(
			const Application& app,
			const KeyboardComponent& keyboard,
			const MouseComponent& mouse
		)
			: Class(app)
			, m_keyboard(keyboard)
			, m_mouse(mouse)
			, m_mouseSensitivity(k_defaultMouseSensitivity)
			, m_rotationRate(k_defaultRotationRate)
			, m_movementRate(k_defaultMovementRate)
			, m_rotationStartPoint(0.f, 0.f)
		{
		}

		FirstPersonCameraComponent::FirstPersonCameraComponent(
			const Application& app,
			const KeyboardComponent& keyboard,
			const MouseComponent& mouse,
			const float fieldOfView,
			const float aspectRatio,
			const float nearPlaneDistance,
			const float farPlaneDistance
		)
			: Class(app, fieldOfView, aspectRatio, nearPlaneDistance, farPlaneDistance)
			, m_keyboard(keyboard)
			, m_mouse(mouse)
			, m_mouseSensitivity(k_defaultMouseSensitivity)
			, m_rotationRate(k_defaultRotationRate)
			, m_movementRate(k_defaultMovementRate)
			, m_rotationStartPoint(0.f, 0.f)
		{
		}

		void FirstPersonCameraComponent::Initialize()
		{
			CameraComponent::Initialize();
		}

		void FirstPersonCameraComponent::Update(const Time& time)
		{
			const auto elapsedTime = time.elapsed.GetMilliseconds<float>();

			// compute movement
			auto movementAmount = math::Vector2::Zero;
			{
				const KeyboardComponent& keyboard = m_keyboard;

				if (keyboard.IsKeyDown(Key::W))
				{
					movementAmount.y += 1.0f;
				}

				if (keyboard.IsKeyDown(Key::S))
				{
					movementAmount.y -= 1.0f;
				}

				if (keyboard.IsKeyDown(Key::A))
				{
					movementAmount.x -= 1.0f;
				}

				if (keyboard.IsKeyDown(Key::D))
				{
					movementAmount.x += 1.0f;
				}
			}

			// compute rotation
			{
				const MouseComponent& mouse = m_mouse;

				if (mouse.WasButtonReleased(MouseButton::Left))
				{
					m_rotationStartPoint.x = 0.f;
					m_rotationStartPoint.y = 0.f;
				}

				if (mouse.WasButtonPressed(MouseButton::Left))
				{
					m_rotationStartPoint.x = mouse.GetX() * m_mouseSensitivity;
					m_rotationStartPoint.y = mouse.GetY() * m_mouseSensitivity;
				}

				if (mouse.IsButtonHeldDown(MouseButton::Left))
				{
					auto prevPoint = m_rotationStartPoint;

					m_rotationStartPoint.x = mouse.GetX() * m_mouseSensitivity;
					m_rotationStartPoint.y = mouse.GetY() * m_mouseSensitivity;

					const auto rotationDelta = m_rotationStartPoint - prevPoint;
					Logger::Info("Rotation delta: %s", rotationDelta.ToString().c_str());

					const auto rotation = rotationDelta * m_rotationRate * elapsedTime;

					const auto pitchMatrix = math::Matrix4::RotationAxis(m_right, rotation.y);
					const auto yawMatrix = math::Matrix4::RotationY(rotation.x);

					ApplyRotation(pitchMatrix * yawMatrix);
				}
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

			CameraComponent::Update(time);
		}

	} // namespace components
} // namespace library
