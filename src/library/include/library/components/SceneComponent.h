#pragma once
#include "library/RTTI.hpp"
#include "library/NonCopyable.hpp"
#include "library/math/Math.h"

namespace library
{
	class CameraComponent;

	class SceneComponent : public NonCopyable<SceneComponent>
	{
		RTTI_CLASS_BASE(SceneComponent)

	public:
		virtual ~SceneComponent();

		const math::Vector3& GetPosition() const { return m_position; }
		const math::Vector3& GetRotation() const { return m_rotation; }
		const math::Vector3& GetScaling() const { return m_scaling; }
		const math::Matrix4& GetWorldMatrix() const { return m_worldMatrix; }

		void SetPosition(const math::Vector3& position);
		void Translate(const math::Vector3& translation);

		void SetRotation(const math::Vector3& rotation);
		void Rotate(const math::Vector3& rotation);

		void SetScaling(const math::Vector3& scaling);
		void Scale(const math::Vector3& scaling);

		const CameraComponent* GetCamera() const { return m_camera; }
		void SetCamera(const CameraComponent& camera);

		const math::Vector3& GetDirection() const { return m_direction; }
		const math::Vector3& GetUp() const { return m_up; }
		const math::Vector3& GetRight() const { return m_right; }

	protected:
		explicit SceneComponent();

	private:
		void UpdateWorldMatrix();

		const CameraComponent* m_camera = nullptr;

		math::Vector3 m_position;
		math::Vector3 m_rotation;
		math::Vector3 m_scaling;

		math::Vector3 m_direction;
		math::Vector3 m_right;
		math::Vector3 m_up;

		math::Matrix4 m_worldMatrix;
	};
} // namespace library
