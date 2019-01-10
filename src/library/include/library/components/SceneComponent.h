#pragma once
#include "library/Component.h"
#include "library/math/Math.h"

namespace library
{
	class Camera;

	class Scene : virtual public Component
	{
		RTTI_CLASS(Scene, Component)

	public:
		~Scene() = default;

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

		const Camera* GetCamera() { return m_camera; }
		void SetCamera(const Camera& camera);

	protected:
		explicit Scene(const Application& app);

		void UpdateWorldMatrix();

		math::Vector3 m_position;
		math::Vector3 m_rotation;
		math::Vector3 m_scaling;

		math::Matrix4 m_worldMatrix;

		const Camera* m_camera = nullptr;
	};

} // namespace library
