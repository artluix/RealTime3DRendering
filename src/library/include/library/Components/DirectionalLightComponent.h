#pragma once
#include "library/Components/LightComponent.h"
#include "library/Math/Vector.h"
#include "library/Math/Quaternion.h"

namespace library
{
class DirectionalLightComponent : public virtual LightComponent
{
	RTTI_CLASS(DirectionalLightComponent, LightComponent)

public:
	DirectionalLightComponent();
	~DirectionalLightComponent();

	const math::Vector3& GetDirection() const { return m_direction; }
	void SetDirection(const math::Vector3& direction);

	void Rotate(const math::Quaternion& rotation);
	void SetRotation(const math::Quaternion& rotation);

#ifdef USE_LIGHT_PROXY_MODEL
	void SetupProxyModel(const CameraComponent& camera) override;
#endif

protected:
	math::Vector3 m_direction;
};
} // namespace library
