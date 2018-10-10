#include "library/components/FirstPersonCameraComponent.h"

#include "library/Math.h"

namespace library
{
    namespace components
    {
        namespace defaults
        {
            constexpr float k_mouseSensitivity = 0.01f;
            const float k_rotationRate = DirectX::XMConvertToRadians(0.2f);
            constexpr float k_movementRate = 1.f;
        }

        FirstPersonCameraComponent::FirstPersonCameraComponent(const Application& app, const KeyboardComponent& keyboard, const MouseComponent& mouse)
            : Class(app)
            , m_keyboard(keyboard)
            , m_mouse(mouse)
            , m_mouseSensitivity(defaults::k_mouseSensitivity)
            , m_rotationRate(defaults::k_rotationRate)
            , m_movementRate(defaults::k_movementRate)
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
            , m_mouseSensitivity(defaults::k_mouseSensitivity)
            , m_rotationRate(defaults::k_rotationRate)
            , m_movementRate(defaults::k_movementRate)
        {
        }

        void FirstPersonCameraComponent::Initialize()
        {
            CameraComponent::Initialize();
        }

        void FirstPersonCameraComponent::Update(const Time& time)
        {
            const auto elapsedTime = time.elapsed.GetMilliseconds<float>();
            const auto right = GetRightVector();

            // compute movement
            auto movementAmount = math::Vector2f::Zero;
            {
                const KeyboardComponent& keyboard = m_keyboard;

                if (keyboard.IsKeyDown(Key::W))
                {
                    movementAmount.y = 1.0f;
                }

                if (keyboard.IsKeyDown(Key::S))
                {
                    movementAmount.y = -1.0f;
                }

                if (keyboard.IsKeyDown(Key::A))
                {
                    movementAmount.x = -1.0f;
                }

                if (keyboard.IsKeyDown(Key::D))
                {
                    movementAmount.x = 1.0f;
                } 
            }

            // compute rotation
            {
                const MouseComponent& mouse = m_mouse;

                if (mouse.IsButtonHeldDown(MouseButton::Left))
                {
                    math::Vector2f rotationAmount;
                    rotationAmount.x = -mouse.GetX() * m_mouseSensitivity;
                    rotationAmount.y = -mouse.GetY() * m_mouseSensitivity;

                    const auto rotationVector = math::Vector(rotationAmount) * m_rotationRate * elapsedTime;

                    const auto pitchMatrix = DirectX::XMMatrixRotationAxis(right, DirectX::XMVectorGetY(rotationVector));
                    const auto yawMatrix = DirectX::XMMatrixRotationY(DirectX::XMVectorGetX(rotationVector));

                    ApplyRotation(DirectX::XMMatrixMultiply(pitchMatrix, yawMatrix));
                }
            }

            auto position = GetPositionVector();
            const auto movement = math::Vector(movementAmount) * m_movementRate * elapsedTime;

            const auto strafe = right * DirectX::XMVectorGetX(movement);
            position += strafe;

            const auto forward = GetDirectionVector() * DirectX::XMVectorGetY(movement);
            position += forward;

            SetPosition(position);

            CameraComponent::Update(time);
        }
    } // namespace components
} // namespace library
