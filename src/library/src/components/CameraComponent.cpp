#include "library/components/CameraComponent.h"

namespace library
{
    namespace components
    {
        namespace defaults
        {
            constexpr float k_fieldOfView = 0.785398163f;
            constexpr float k_nearPlaneDistance = 0.01f;
            constexpr float k_farPlaneDistance = 0.01f;
        }

        Camera::Camera(const Application& app)
            : Class(app)
            , m_fieldOfView(defaults::k_fieldOfView)
            , m_nearPlaneDistance(defaults::k_nearPlaneDistance)
            , m_farPlaneDistance(defaults::k_farPlaneDistance)
        {
        }

        Camera::Camera(const Application& app, const float fieldOfView, const float aspectRatio, const float nearPlaneDistance, const float farPlaneDistance)
            : Class(app)
            , m_fieldOfView(fieldOfView)
            , m_nearPlaneDistance(nearPlaneDistance)
            , m_farPlaneDistance(farPlaneDistance)
        {
        }

        /////////////////////////////////////////////////////////////////

        DirectX::XMVECTOR Camera::GetPositionVector() const
        {
            return DirectX::XMLoadFloat3(&m_position);
        }

        DirectX::XMVECTOR Camera::GetDirectionVector() const
        {
            return DirectX::XMLoadFloat3(&m_direction);
        }

        DirectX::XMVECTOR Camera::GetUpVector() const
        {
            return DirectX::XMLoadFloat3(&m_up);
        }

        DirectX::XMVECTOR Camera::GetRightVector() const
        {
            return DirectX::XMLoadFloat3(&m_right);
        }

        /////////////////////////////////////////////////////////////////

        DirectX::XMMATRIX Camera::GetViewMatrix() const
        {
            return DirectX::XMLoadFloat4x4(&m_viewMatrix);
        }

        DirectX::XMMATRIX Camera::GetProjectionMatrix() const
        {
            return DirectX::XMLoadFloat4x4(&m_projectionMatrix);
        }

        DirectX::XMMATRIX Camera::GetViewProjectionMatrix() const
        {
            const auto viewMatrix = GetViewMatrix();
            const auto projectionMatrix = GetProjectionMatrix();

            return DirectX::XMMatrixMultiply(viewMatrix, projectionMatrix);
        }

        /////////////////////////////////////////////////////////////////

        void Camera::SetPosition(const float x, const float y, const float z)
        {
            m_position = DirectX::XMFLOAT3(x, y, z);
        }

        void Camera::SetPosition(DirectX::FXMVECTOR position)
        {
            DirectX::XMStoreFloat3(&m_position, position);
        }

        void Camera::SetPosition(const DirectX::XMFLOAT3& position)
        {
            m_position = position;
        }

        /////////////////////////////////////////////////////////////////

        void Camera::Reset()
        {

        }

        /////////////////////////////////////////////////////////////////

        void Camera::Initialize()
        {

        }

        void Camera::Update(const Time& time)
        {

        }

        /////////////////////////////////////////////////////////////////


    }
}
