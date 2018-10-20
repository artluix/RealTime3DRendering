#pragma once
#include "library/components/CameraComponent.h"

#include <functional>

namespace library
{
    namespace components
    {
        class MouseComponent;
        class KeyboardComponent;

        class FirstPersonCameraComponent : public rtti::Class<FirstPersonCameraComponent, CameraComponent>
        {
        public:
            explicit FirstPersonCameraComponent(const Application& app, const KeyboardComponent& keyboard, const MouseComponent& mouse);
            explicit FirstPersonCameraComponent(
                const Application& app,
                const KeyboardComponent& keyboard,
                const MouseComponent& mouse,
                const float fieldOfView,
                const float aspectRatio,
                const float nearPlaneDistance,
                const float farPlaneDistance
            );

            ~FirstPersonCameraComponent() = default;

            const KeyboardComponent& GetKeyboard() const { return m_keyboard; }
            void SetKeyboard(const KeyboardComponent& keyboard) { m_keyboard = keyboard; }

            const MouseComponent& GetMouse() const { return m_mouse; }
            void SetMouse(const MouseComponent& mouse) { m_mouse = mouse; }

            float GetMouseSensitivity() const { return m_mouseSensitivity; }
            void SetMouseSensitivity(const float sensitivity) { m_mouseSensitivity = sensitivity; }

            float GetRotationRate() const { return m_rotationRate; }
            void SetRotationRate(const float rotationRate) { m_rotationRate = rotationRate; }

            float GetMovementRate() const { return m_movementRate; }
            void SetMovementRate(const float movementRate) { m_movementRate = movementRate; }

            void Initialize() override;
            void Update(const Time& time) override;

        protected:
            std::reference_wrapper<const KeyboardComponent> m_keyboard;
            std::reference_wrapper<const MouseComponent> m_mouse;

            math::Vector2f m_rotationStartPoint;

            float m_mouseSensitivity;
            float m_rotationRate;
            float m_movementRate;
        };
    } // namespace components
} // namespace library
