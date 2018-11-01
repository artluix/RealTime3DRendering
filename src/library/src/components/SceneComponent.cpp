#include "library/components/SceneComponent.h"

#include "library/components/CameraComponent.h"

namespace library
{
	namespace components
	{

		SceneComponent::SceneComponent(const Application& app, const CameraComponent& camera)
			: Class(app)
			, m_camera(camera)
		{
			UpdateWorldMatrix();
		}

		void SceneComponent::SetCamera(const CameraComponent& camera)
		{
			m_camera = camera;
		}

		void SceneComponent::SetPosition(const math::Vector3& position)
		{
			if (m_position != position)
			{
				m_position = position;
				UpdateWorldMatrix();
			}
		}

		void SceneComponent::Translate(const math::Vector3& translation)
		{
			if (!translation)
				return;

			SetPosition(m_position + translation);
		}

		void SceneComponent::SetRotation(const math::Vector3& rotation)
		{
			if (m_rotation != rotation)
			{
				m_rotation = rotation;
				UpdateWorldMatrix();
			}
		}

		void SceneComponent::Rotate(const math::Vector3& rotation)
		{
			if (!rotation)
				return;

			SetRotation(m_rotation + rotation);
		}

		void SceneComponent::UpdateWorldMatrix()
		{
			const auto translationMatrix = math::Matrix4::Translation(m_position);
			const auto rotationMatrix = math::Matrix4::RotationRollPitchYaw(m_rotation);

			m_worldMatrix = rotationMatrix * translationMatrix;
		}

	} // namespace components
} // namespace library
