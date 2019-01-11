#include "StdAfx.h"
#include "library/components/SceneComponent.h"

namespace library
{
	SceneComponent::SceneComponent(const Application& app)
		: DrawableComponent(app)
		, m_scaling(math::Vector3::One)
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
		const auto rotationMatrix = math::Matrix4::RotationRollPitchYaw(m_rotation);
		const auto scalingMatrix = math::Matrix4::Scaling(m_scaling);

		m_worldMatrix = scalingMatrix * rotationMatrix * translationMatrix;
	}
} // namespace library
