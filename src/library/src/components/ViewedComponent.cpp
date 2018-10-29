#include "library/components/ViewedComponent.h"

#include "library/components/CameraComponent.h"

namespace library
{
	namespace components
	{

		ViewedComponent::ViewedComponent(const Application& app, const CameraComponent& camera)
			: Class(app)
			, m_camera(camera)
		{
			UpdateWorldMatrix();
		}

		void ViewedComponent::SetCamera(const CameraComponent& camera)
		{
			m_camera = camera;
		}

		void ViewedComponent::SetPosition(const math::Vector3& position)
		{
			if (m_position != position)
			{
				m_position = position;
				UpdateWorldMatrix();
			}
		}

		void ViewedComponent::Translate(const math::Vector3& translation)
		{
			if (!translation)
				return;

			SetPosition(m_position + translation);
		}

		void ViewedComponent::SetRotation(const math::Vector3& rotation)
		{
			if (m_rotation != rotation)
			{
				m_rotation = rotation;
				UpdateWorldMatrix();
			}
		}

		void ViewedComponent::Rotate(const math::Vector3& rotation)
		{
			if (!rotation)
				return;

			SetRotation(m_rotation + rotation);
		}

		void ViewedComponent::UpdateWorldMatrix()
		{
			const auto translationMatrix = math::Matrix4::Translation(m_position);
			const auto rotationMatrix = math::Matrix4::RotationRollPitchYaw(m_rotation);

			m_worldMatrix = rotationMatrix * translationMatrix;
		}

	} // namespace components
} // namespace library
