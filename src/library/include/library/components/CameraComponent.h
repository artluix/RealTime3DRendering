#pragma once
#include "library/components/BaseComponent.h"
#include "library/RTTI.hpp"

#include <DirectXMath.h>

namespace library
{
    namespace components
    {
        class Camera : public rtti::Class<Camera, Base>
        {
        public:
            explicit Camera(const Application& app);
            explicit Camera(
                const Application& app,
                const float fieldOfView,
                const float aspectRatio,
                const float nearPlaneDistance,
                const float farPlaneDistance
            );

            ~Camera() = default;

            // XMFLOAT3 wrappers
            const DirectX::XMFLOAT3& GetPosition() const { return m_position; }
            const DirectX::XMFLOAT3& GetDirection() const { return m_direction; }
            const DirectX::XMFLOAT3& GetUp() const { return m_up; }
            const DirectX::XMFLOAT3& GetRight() const { return m_right; }

            // XMVECTOR wrappers
            DirectX::XMVECTOR GetPositionVector() const;
            DirectX::XMVECTOR GetDirectionVector() const;
            DirectX::XMVECTOR GetUpVector() const;
            DirectX::XMVECTOR GetRightVector() const;

            float GetAspectRatio() const { return m_aspectRatio; }
            float GetFieldOfView() const { return m_fieldOfView; }
            float GetNearPlaneDistance() const { return m_nearPlaneDistance; }
            float GetFarPlaneDistance() const { return m_farPlaneDistance; }

            DirectX::XMMATRIX GetViewMatrix() const;
            DirectX::XMMATRIX GetProjectionMatrix() const;
            DirectX::XMMATRIX GetViewProjectionMatrix() const;

            // types begins with XM - default ones
            // types begins with {C}XM - aliases, check them before passing

            virtual void SetPosition(const float x, const float y, const float z);
            virtual void SetPosition(DirectX::FXMVECTOR position);
            virtual void SetPosition(const DirectX::XMFLOAT3& position);

            virtual void Reset();

            void Initialize() override;
            void Update(const Time& time) override;

            virtual void UpdateViewMatrix();
            virtual void UpdateProjectionMatrix();

            void ApplyRotation(DirectX::CXMMATRIX transform);
            void ApplyRotation(const DirectX::XMFLOAT4X4& transform);

        protected:
            float m_fieldOfView;
            float m_aspectRatio;
            float m_nearPlaneDistance;
            float m_farPlaneDistance;

            DirectX::XMFLOAT3 m_position;
            DirectX::XMFLOAT3 m_direction;
            DirectX::XMFLOAT3 m_up;
            DirectX::XMFLOAT3 m_right;

            DirectX::XMFLOAT4X4 m_viewMatrix;
            DirectX::XMFLOAT4X4 m_projectionMatrix;
        };
    }
}

