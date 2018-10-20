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
        namespace defaults
        {
            constexpr float k_mouseSensitivity = 0.1f;
            const float k_rotationRate = DirectX::XMConvertToRadians(0.5f);
            constexpr float k_movementRate = 0.1f;
        }

        FirstPersonCameraComponent::FirstPersonCameraComponent(const Application& app, const KeyboardComponent& keyboard, const MouseComponent& mouse)
            : Class(app)
            , m_keyboard(keyboard)
            , m_mouse(mouse)
            , m_mouseSensitivity(defaults::k_mouseSensitivity)
            , m_rotationRate(defaults::k_rotationRate)
            , m_movementRate(defaults::k_movementRate)
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
            , m_mouseSensitivity(defaults::k_mouseSensitivity)
            , m_rotationRate(defaults::k_rotationRate)
            , m_movementRate(defaults::k_movementRate)
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
            const auto right = GetRightVector();

            // compute movement
            bool isMoved = false;
            auto movementAmount = math::Vector2f::Zero;
            {
                const KeyboardComponent& keyboard = m_keyboard;

                if (keyboard.IsKeyDown(Key::W))
                {
                    movementAmount.y = 1.0f;
                    isMoved = true;
                }

                if (keyboard.IsKeyDown(Key::S))
                {
                    movementAmount.y = -1.0f;
                    isMoved = true;
                }

                if (keyboard.IsKeyDown(Key::A))
                {
                    movementAmount.x = -1.0f;
                    isMoved = true;
                }

                if (keyboard.IsKeyDown(Key::D))
                {
                    movementAmount.x = 1.0f;
                    isMoved = true;
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

                    math::Vector2f rotationDelta(m_rotationStartPoint.x - prevPoint.x, m_rotationStartPoint.y -prevPoint.y);
                    Logger::Info("Rotation delta: %s", rotationDelta.ToString().c_str());

                    const auto rotationVector = math::Vector(rotationDelta) * m_rotationRate * elapsedTime;

                    const auto pitchMatrix = DirectX::XMMatrixRotationAxis(right, DirectX::XMVectorGetY(rotationVector));
                    const auto yawMatrix = DirectX::XMMatrixRotationY(DirectX::XMVectorGetX(rotationVector));

                    ApplyRotation(DirectX::XMMatrixMultiply(pitchMatrix, yawMatrix));
                }
            }

            if (isMoved)
            {
                auto position = GetPositionVector();
                const auto movement = math::Vector(movementAmount) * m_movementRate * elapsedTime;

                const auto strafe = right * DirectX::XMVectorGetX(movement);
                position += strafe;

                const auto forward = GetDirectionVector() * DirectX::XMVectorGetY(movement);
                position += forward;

                SetPosition(position);
            }

            CameraComponent::Update(time);
        }
    } // namespace components
} // namespace library
