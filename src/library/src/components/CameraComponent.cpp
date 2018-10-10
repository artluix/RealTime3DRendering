#include "library/components/CameraComponent.h"

#include <sstream>

#define _USE_MATH_DEFINES
#include <math.h>

namespace library
{
    namespace
    {
        namespace defaults
        {
            constexpr float k_fieldOfView = float(M_PI_4);
            constexpr float k_nearPlaneDistance = 0.01f;
            constexpr float k_farPlaneDistance = 1000.0f;
        }
    }

    namespace components
    {
        CameraComponent::CameraComponent(const Application& app)
            : Class(app)
            , m_aspectRatio(app.GetAspectRatio())
            , m_fieldOfView(defaults::k_fieldOfView)
            , m_nearPlaneDistance(defaults::k_nearPlaneDistance)
            , m_farPlaneDistance(defaults::k_farPlaneDistance)
            , m_isViewMatrixDirty(true)
            , m_position(), m_direction(), m_right(), m_up()
            , m_viewMatrix(), m_projectionMatrix()
        {
        }

        CameraComponent::CameraComponent(const Application& app, const float fieldOfView, const float aspectRatio, const float nearPlaneDistance, const float farPlaneDistance)
            : Class(app)
            , m_fieldOfView(fieldOfView)
            , m_nearPlaneDistance(nearPlaneDistance)
            , m_farPlaneDistance(farPlaneDistance)
        {
        }

        /////////////////////////////////////////////////////////////////

        DirectX::XMMATRIX CameraComponent::GetViewProjectionMatrix() const
        {
            const auto viewMatrix = GetViewMatrix();
            const auto projectionMatrix = GetProjectionMatrix();

            return DirectX::XMMatrixMultiply(viewMatrix, projectionMatrix);
        }

        /////////////////////////////////////////////////////////////////

        void CameraComponent::SetPosition(const float x, const float y, const float z)
        {
            const auto position = math::Vector3f(x, y, z);
            SetPosition(position);
        }

        void CameraComponent::SetPosition(const math::Vector& positionVector)
        {
            const auto position = math::Vector3f(positionVector);
            SetPosition(position);
        }

        void CameraComponent::SetPosition(const math::Vector3f& position)
        {
            if (m_position != position)
            {
                std::string message = "Position changed: " + position.ToString();
                OutputDebugStringA(message.c_str());
                OutputDebugStringA("\n");

                m_position = position;
                m_isViewMatrixDirty = true;
            }
        }

        /////////////////////////////////////////////////////////////////

        void CameraComponent::Reset()
        {
            m_position = math::Vector3f::Zero;
            m_direction = math::Vector3f::Forward;
            m_up = math::Vector3f::Up;
            m_right = math::Vector3f::Right;

            UpdateViewMatrix();
        }

        /////////////////////////////////////////////////////////////////

        void CameraComponent::Initialize()
        {
            UpdateProjectionMatrix();
            Reset();
        }

        void CameraComponent::Update(const Time& time)
        {
            if (m_isViewMatrixDirty)
            {
                UpdateViewMatrix();
                m_isViewMatrixDirty = false;
            }
        }

        /////////////////////////////////////////////////////////////////

        void CameraComponent::UpdateViewMatrix()
        {
            const auto eyePosition = GetPositionVector();
            const auto direction = GetDirectionVector();
            const auto upDirection = GetUpVector();

            const auto viewMatrix = DirectX::XMMatrixLookAtRH(eyePosition, direction, upDirection);
            m_viewMatrix = math::Matrix4(viewMatrix);
        }

        void CameraComponent::UpdateProjectionMatrix()
        {
            const auto projectionMatrix = DirectX::XMMatrixPerspectiveFovRH(
                m_fieldOfView,
                m_aspectRatio,
                m_nearPlaneDistance,
                m_farPlaneDistance
            );
            m_projectionMatrix = math::Matrix4(projectionMatrix);
        }

        /////////////////////////////////////////////////////////////////

        void CameraComponent::ApplyRotation(const math::Matrix& transform)
        {
            auto direction = GetDirectionVector();
            auto up = GetUpVector();

            direction = DirectX::XMVector3TransformNormal(direction, transform);
            direction = DirectX::XMVector3Normalize(direction);

            up = DirectX::XMVector3TransformNormal(up, transform);
            up = DirectX::XMVector3Normalize(up);

            const auto right = DirectX::XMVector3Cross(direction, up);
            up = DirectX::XMVector3Cross(right, direction);

            const auto newDirection = math::Vector3f(direction);
            if (m_direction != newDirection)
            {
                m_direction = newDirection;
                m_isViewMatrixDirty = true;
            }

            const auto newUp = math::Vector3f(up);
            if (m_up != newUp)
            {
                m_up = newUp;
                m_isViewMatrixDirty = true;
            }

            const auto newRight = math::Vector3f(right);
            if (m_right != newRight)
            {
                m_right = newRight;
                m_isViewMatrixDirty = true;
            }
        }

        void CameraComponent::ApplyRotation(const math::Matrix4& transform)
        {
            const auto transformMatrix = math::Matrix(transform);
            ApplyRotation(transformMatrix);
        }
    } // namespace components
} // namespace library
