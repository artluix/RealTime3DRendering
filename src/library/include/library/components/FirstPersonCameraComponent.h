#pragma once
#include "library/components/CameraComponent.h"

namespace library
{
	class MouseComponent;
	class KeyboardComponent;

	class FirstPersonCameraComponent : public CameraComponent
	{
		RTTI_CLASS(FirstPersonCameraComponent, CameraComponent)

	public:
		explicit FirstPersonCameraComponent(const Application& app);
		explicit FirstPersonCameraComponent(
			const Application& app,
			const float fieldOfView,
			const float aspectRatio,
			const float nearPlaneDistance,
			const float farPlaneDistance
		);

		~FirstPersonCameraComponent() = default;

		const KeyboardComponent* GetKeyboard() const { return m_keyboard; }
		void SetKeyboard(const KeyboardComponent& keyboard);

		const MouseComponent* GetMouse() const { return m_mouse; }
		void SetMouse(const MouseComponent& mouse);

		float GetMouseSensitivity() const { return m_mouseSensitivity; }
		void SetMouseSensitivity(const float sensitivity) { m_mouseSensitivity = sensitivity; }

		float GetRotationRate() const { return m_rotationRate; }
		void SetRotationRate(const float rotationRate) { m_rotationRate = rotationRate; }

		float GetMovementRate() const { return m_movementRate; }
		void SetMovementRate(const float movementRate) { m_movementRate = movementRate; }

		void Initialize() override;
		void Update(const Time& time) override;

	protected:
		const KeyboardComponent* m_keyboard = nullptr;
		const MouseComponent* m_mouse = nullptr;

		math::Vector2 m_rotationStartPoint;

		float m_mouseSensitivity;
		float m_rotationRate;
		float m_movementRate;
	};

} // namespace library
