#include "StdAfx.h"
#include "library/components/SceneComponent.h"

namespace library
{
	SceneComponent::SceneComponent(const Application& app, const CameraComponent* camera /*= nullptr*/)
		: DrawableComponent(app)
		, m_scaling(math::Vector3::One)
		, m_worldMatrix(math::Matrix4::Identity)
		, m_direction(math::Vector3::Forward)
		, m_up(math::Vector3::Up)
		, m_right(math::Vector3::Right)
	{
		UpdateWorldMatrix();
	}

	SceneComponent::~SceneComponent() = default;
	
	//-------------------------------------------------------------------------

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

	//-------------------------------------------------------------------------

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

	//-------------------------------------------------------------------------

	void SceneComponent::SetScaling(const math::Vector3& scaling)
	{
		if (scaling < math::Vector3::Zero)
			return;

		if (m_scaling != scaling)
		{
			m_scaling = scaling;
			UpdateWorldMatrix();
		}
	}

	void SceneComponent::Scale(const math::Vector3& scaling)
	{
		if (!scaling)
			return;

		SetScaling(m_scaling + scaling);
	}

	//-------------------------------------------------------------------------

	void SceneComponent::UpdateWorldMatrix()
	{
		const auto translationMatrix = math::Matrix4::Translation(m_position);
		const auto rotationMatrix = math::Matrix4::RotationPitchYawRoll(m_rotation);
		const auto scalingMatrix = math::Matrix4::Scaling(m_scaling);

		m_direction = rotationMatrix.GetForward();
		m_up = rotationMatrix.GetUp();
		m_right = rotationMatrix.GetRight();

		m_worldMatrix = scalingMatrix * rotationMatrix * translationMatrix;

		m_direction = rotationMatrix.GetForward();
		m_up = rotationMatrix.GetUp();
		m_right = rotationMatrix.GetRight();
	}
} // namespace library
