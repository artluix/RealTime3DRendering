#pragma once
#include "library/components/DrawableComponent.h"
#include "library/Math.h"

namespace library
{
	namespace components
	{

		class CameraComponent;

		class SceneComponent : public rtti::Class<SceneComponent, DrawableComponent>
		{
		public:
			explicit SceneComponent(const Application& app, const CameraComponent& camera);
			~SceneComponent() = default;

			const CameraComponent& GetCamera() { return m_camera; }
			void SetCamera(const CameraComponent& camera);

			const math::Vector3& GetPosition() const { return m_position; }
			const math::Vector3& GetRotation() const { return m_rotation; }
			const math::Matrix4& GetWorldMatrix() const { return m_worldMatrix; }

			void SetPosition(const math::Vector3& position);
			void Translate(const math::Vector3& translation);

			void SetRotation(const math::Vector3& rotation);
			void Rotate(const math::Vector3& rotation);

		protected:
			void UpdateWorldMatrix();

			math::Vector3 m_position;
			math::Vector3 m_rotation;
			math::Matrix4 m_worldMatrix;

			std::reference_wrapper<const CameraComponent> m_camera;
		};

	} // namespace components
} // namespace library
