#pragma once
#include "library/Math.h"

namespace library
{
	class SceneComponent
	{
	public:
		explicit SceneComponent();
		~SceneComponent() = default;

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

	protected:
		void UpdateWorldMatrix();

		math::Vector3 m_position;
		math::Vector3 m_rotation;
		math::Vector3 m_scaling;

		math::Matrix4 m_worldMatrix;
	};

} // namespace library
