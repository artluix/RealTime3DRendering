#pragma once
#include "library/Components/LightComponent.h"
#include "library/Math/Vector.h"

namespace library
{
class PointLightComponent : public virtual LightComponent
{
	RTTI_CLASS(PointLightComponent, LightComponent)

public:
	PointLightComponent();
	~PointLightComponent();

	const math::Vector3& GetPosition() const { return m_position; }
	void SetPosition(const math::Vector3& position);

	float GetRadius() const { return m_radius; }
	void SetRadius(const float radius);

	bool IsVisibleFrom(const math::Vector3& positionFrom) const;

#ifdef USE_LIGHT_PROXY_MODEL
	void SetupProxyModel(const CameraComponent& camera) override;
#endif

protected:
	math::Vector3 m_position;
	float m_radius;
};
} // namespace library
