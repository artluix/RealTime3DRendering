#pragma once
#include "library/components/Component.h"
#include "library/math/Math.h"

namespace library
{
	class CameraComponent : public virtual Component
	{
		RTTI_CLASS(CameraComponent, Component)

	public:
		explicit CameraComponent();
		explicit CameraComponent(
			const float fieldOfView,
			const float aspectRatio,
			const float nearPlaneDistance,
			const float farPlaneDistance
		);

		~CameraComponent();

		const math::Vector3& GetPosition() const { return m_position; }
		const math::Vector3& GetDirection() const { return m_direction; }
		const math::Vector3& GetUp() const { return m_up; }
		const math::Vector3& GetRight() const { return m_right; }

		float GetAspectRatio() const { return m_aspectRatio; }
		float GetFieldOfView() const { return m_fieldOfView; }
		float GetNearPlaneDistance() const { return m_nearPlaneDistance; }
		float GetFarPlaneDistance() const { return m_farPlaneDistance; }

		const math::Matrix4& GetViewMatrix() const { return m_viewMatrix; }
		const math::Matrix4& GetProjectionMatrix() const { return m_projectionMatrix; }
		math::Matrix4 GetViewProjectionMatrix() const;

		virtual void SetPosition(const math::Vector3& position);
		virtual void Reset();

		void Initialize(const Application& app) override;
		void Update(const Time& time) override;

		virtual void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix();

		void ApplyRotation(const math::Matrix4& transform);

	private:
		float m_fieldOfView;
		float m_aspectRatio;
		float m_nearPlaneDistance;
		float m_farPlaneDistance;

		math::Vector3 m_position;

		math::Vector3 m_direction;
		math::Vector3 m_up;
		math::Vector3 m_right;

		math::Matrix4 m_viewMatrix;
		bool m_isViewMatrixDirty;

		math::Matrix4 m_projectionMatrix;
	};
} // namespace library

