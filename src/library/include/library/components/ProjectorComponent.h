#pragma once
#include "library/Components/Component.h"
#include "library/Math/Math.h"

namespace library
{
	class ProjectorComponent : public virtual Component
	{
		RTTI_CLASS(ProjectorComponent, Component)

	public:
		explicit ProjectorComponent();
		explicit ProjectorComponent(
			const float fieldOfView,
			const float aspectRatio,
			const float nearPlaneDistance,
			const float farPlaneDistance
		);

		~ProjectorComponent();

		const math::Vector3& GetPosition() const { return m_position; }
		virtual void SetPosition(const math::Vector3& position);

		const math::Vector3& GetDirection() const { return m_direction; }
		const math::Vector3& GetUp() const { return m_up; }
		const math::Vector3& GetRight() const { return m_right; }

		float GetAspectRatio() const { return m_aspectRatio; }
		void SetAspectRatio(const float aspectRatio);

		float GetFieldOfView() const { return m_fieldOfView; }
		void SetFieldOfView(const float fieldOfView);
		
		float GetNearPlaneDistance() const { return m_nearPlaneDistance; }
		void SetNearPlaneDistance(const float nearPlaneDistance);

		float GetFarPlaneDistance() const { return m_farPlaneDistance; }
		void SetFarPlaneDistance(const float farPlaneDistance);

		const math::Matrix4& GetViewMatrix() const { assert(!m_isViewMatrixDirty); return m_viewMatrix; }
		const math::Matrix4& GetProjectionMatrix() const { assert(!m_isProjectionMatrixDirty); return m_projectionMatrix; }
		const math::Matrix4& GetViewProjectionMatrix() const { assert(!m_isViewMatrixDirty && !m_isProjectionMatrixDirty); return m_viewProjectionMatrix; }

		void Initialize(const Application& app) override;
		void Update(const Time& time) override;

		virtual void Reset();

		virtual bool UpdateViewMatrix();
		virtual bool UpdateProjectionMatrix();

		void ApplyRotation(const math::Matrix4& transform);

	protected:
		float m_fieldOfView;
		float m_aspectRatio;
		float m_nearPlaneDistance;
		float m_farPlaneDistance;

	private:
		virtual void UpdateViewProjectionMatrix();

		math::Vector3 m_position;

		math::Vector3 m_direction;
		math::Vector3 m_up;
		math::Vector3 m_right;

		math::Matrix4 m_viewMatrix;
		bool m_isViewMatrixDirty = true;

		math::Matrix4 m_projectionMatrix;
		bool m_isProjectionMatrixDirty = true;

		math::Matrix4 m_viewProjectionMatrix;
	};
} // namespace library

