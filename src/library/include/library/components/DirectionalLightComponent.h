#pragma once
#include "library/Components/LightComponent.h"
#include "library/Math/Vector.h"
#include "library/Math/Quaternion.h"

namespace library
{
class DirectionalLightComponent : public virtual LightComponent
{
public:
	DirectionalLightComponent();
	~DirectionalLightComponent();

	const math::Vector3& GetDirection() const { return m_direction; }
	const math::Vector3& GetUp() const { return m_up; }
	const math::Vector3& GetRight() const { return m_right; }
	const math::Quaternion& GetRotation() const { return m_rotation; }

	void Rotate(const math::Quaternion& rotation);
	void SetRotation(const math::Quaternion& rotation);

private:
	math::Vector3 m_direction;
	math::Vector3 m_up;
	math::Vector3 m_right;

	math::Quaternion m_rotation;
};
} // namespace library
