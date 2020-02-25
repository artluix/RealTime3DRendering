#pragma once
#include "library/Components/Component.h"
#include "library/Math/Transform.h"
#include "library/Math/Matrix.h"

namespace library
{
class SceneComponent : public virtual Component
{
	RTTI_CLASS(SceneComponent, Component)

public:
	SceneComponent();
	~SceneComponent();

	const math::Transform& GetTransform() const { return m_transform; }

	const math::Vector3& GetPosition() const { return m_transform.position; }
	const math::Quaternion& GetRotation() const { return m_transform.rotation; }
	const math::Vector3& GetScaling() const { return m_transform.scaling; }

	const math::Matrix4& GetInitialTransformMatrix() const { return m_initialTransformMatrix; }
	const math::Matrix4& GetWorldMatrix() const;

	void SetPosition(const math::Vector3& position);
	void Translate(const math::Vector3& translation);

	void SetRotation(const math::Quaternion& rotation);
	void Rotate(const math::Quaternion& rotation);

	void SetRotation(const math::Vector3& pitchYawRoll);
	void Rotate(const math::Vector3& pitchYawRoll);

	void SetScaling(const math::Vector3& scaling);
	void Scale(const math::Vector3& scaling);

	void SetInitialTransform(const math::Matrix4& initialTransform);

	const math::Vector3& GetDirection() const { return m_direction; }
	const math::Vector3& GetUp() const { return m_up; }
	const math::Vector3& GetRight() const { return m_right; }

	//-------------------------------------------------------------------------

	void Update(const Time& time) override;

protected:
	void UpdateWorldMatrix();

	virtual void OnTranslation(const math::Vector3& translationDelta) {}
	virtual void OnRotation(const math::Quaternion& rotationDelta) {}
	virtual void OnScaling(const math::Vector3& scalingDelta) {}

	math::Transform m_transform;

	math::Vector3 m_direction;
	math::Vector3 m_right;
	math::Vector3 m_up;

	math::Matrix4 m_initialTransformMatrix = math::Matrix4::Identity;

	math::Matrix4 m_worldMatrix = math::Matrix4::Identity;
	bool m_isWorldMatrixDirty = true;
};
} // namespace library
