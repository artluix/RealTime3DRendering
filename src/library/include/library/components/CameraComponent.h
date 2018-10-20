#pragma once
#include "library/components/BaseComponent.h"
#include "library/RTTI.hpp"
#include "library/Math.h"

namespace library
{
    namespace components
    {
        class CameraComponent : public rtti::Class<CameraComponent, BaseComponent>
        {
        public:
            explicit CameraComponent(const Application& app);
            explicit CameraComponent(
                const Application& app,
                const float fieldOfView,
                const float aspectRatio,
                const float nearPlaneDistance,
                const float farPlaneDistance
            );

            ~CameraComponent();

            // XMFLOAT3 wrappers
            const math::Vector3f& GetPosition() const { return m_position; }
            const math::Vector3f& GetDirection() const { return m_direction; }
            const math::Vector3f& GetUp() const { return m_up; }
            const math::Vector3f& GetRight() const { return m_right; }

            // XMVECTOR wrappers
            math::Vector GetPositionVector() const { return math::Vector(m_position); }
            math::Vector GetDirectionVector() const { return math::Vector(m_direction); }
            math::Vector GetUpVector() const { return math::Vector(m_up); }
            math::Vector GetRightVector() const { return math::Vector(m_right); }

            float GetAspectRatio() const { return m_aspectRatio; }
            float GetFieldOfView() const { return m_fieldOfView; }
            float GetNearPlaneDistance() const { return m_nearPlaneDistance; }
            float GetFarPlaneDistance() const { return m_farPlaneDistance; }

            math::Matrix GetViewMatrix() const { return math::Matrix(m_viewMatrix); }
            math::Matrix GetProjectionMatrix() const { return math::Matrix(m_projectionMatrix); }
            math::Matrix GetViewProjectionMatrix() const;

            // types begins with XM - default ones
            // types begins with {C}XM - aliases, check them before passing

            virtual void SetPosition(const float x, const float y, const float z);
            virtual void SetPosition(const math::Vector& positionVector);
            virtual void SetPosition(const math::Vector3f& position);

            virtual void Reset();

            void Initialize() override;
            void Update(const Time& time) override;

            virtual void UpdateViewMatrix();
            virtual void UpdateProjectionMatrix();

            void ApplyRotation(const math::Matrix& transform);
            void ApplyRotation(const math::Matrix4& transform);

        protected:
            float m_fieldOfView;
            float m_aspectRatio;
            float m_nearPlaneDistance;
            float m_farPlaneDistance;

            math::Vector3f m_position;

            math::Vector3f m_direction;
            math::Vector3f m_up;
            math::Vector3f m_right;

            math::Matrix4 m_viewMatrix;
            bool m_isViewMatrixDirty;

            math::Matrix4 m_projectionMatrix;
        };
    } // namespace components
} // namespace library

