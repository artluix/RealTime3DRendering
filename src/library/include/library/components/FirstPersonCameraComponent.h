#pragma once
#include "library/components/CameraComponent.h"
#include "library/components/InputReceivableComponent.h"

namespace library
{
	class FirstPersonCameraComponent
		: public CameraComponent
		, public InputReceivableComponent
	{
		RTTI_CLASS(FirstPersonCameraComponent, CameraComponent, InputReceivableComponent)

	public:
		explicit FirstPersonCameraComponent();
		explicit FirstPersonCameraComponent(
			const float fieldOfView,
			const float aspectRatio,
			const float nearPlaneDistance,
			const float farPlaneDistance
		);

		~FirstPersonCameraComponent() = default;

		float GetMouseSensitivity() const { return m_mouseSensitivity; }
		void SetMouseSensitivity(const float sensitivity) { m_mouseSensitivity = sensitivity; }

		float GetRotationRate() const { return m_rotationRate; }
		void SetRotationRate(const float rotationRate) { m_rotationRate = rotationRate; }

		float GetMovementRate() const { return m_movementRate; }
		void SetMovementRate(const float movementRate) { m_movementRate = movementRate; }

		void Update(const Time& time) override;

	protected:
		math::Vector2 m_rotationStartPoint;

		float m_mouseSensitivity;
		float m_rotationRate;
		float m_movementRate;
	};
} // namespace library
