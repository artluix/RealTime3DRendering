#include "StdAfx.h"
#include "library/components/SceneComponent.h"

namespace library
{
	Scene::Scene(const Application& app)
		: Component(app)
		, m_scaling(math::constants::Vector3::One)
	{
		UpdateWorldMatrix();
	}

	//-------------------------------------------------------------------------

	void Scene::SetPosition(const math::Vector3& position)
	{
		if (m_position != position)
		{
			m_position = position;
			UpdateWorldMatrix();
		}
	}

	void Scene::Translate(const math::Vector3& translation)
	{
		if (!translation)
			return;

		SetPosition(m_position + translation);
	}

	//-------------------------------------------------------------------------

	void Scene::SetRotation(const math::Vector3& rotation)
	{
		if (m_rotation != rotation)
		{
			m_rotation = rotation;
			UpdateWorldMatrix();
		}
	}

	void Scene::Rotate(const math::Vector3& rotation)
	{
		if (!rotation)
			return;

		SetRotation(m_rotation + rotation);
	}

	//-------------------------------------------------------------------------

	void Scene::SetScaling(const math::Vector3& scaling)
	{
		if (scaling < math::constants::Vector3::Zero)
			return;

		if (m_scaling != scaling)
		{
			m_scaling = scaling;
			UpdateWorldMatrix();
		}
	}

	void Scene::Scale(const math::Vector3& scaling)
	{
		if (!scaling)
			return;

		SetScaling(m_scaling + scaling);
	}

	//-------------------------------------------------------------------------

	void Scene::SetCamera(const Camera& camera)
	{
		if (m_camera != &camera)
			m_camera = &camera;
	}

	//-------------------------------------------------------------------------

	void Scene::UpdateWorldMatrix()
	{
		const auto translationMatrix = math::Matrix4::Translation(m_position);
		const auto rotationMatrix = math::Matrix4::RotationRollPitchYaw(m_rotation);
		const auto scalingMatrix = math::Matrix4::Scaling(m_scaling);

		m_worldMatrix = scalingMatrix * rotationMatrix * translationMatrix;
	}

} // namespace library
