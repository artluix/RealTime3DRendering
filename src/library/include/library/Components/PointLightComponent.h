#pragma once
#include "library/Components/LightComponent.h"
#include "library/Math/Vector.h"

namespace library
{
__declspec(align(16)) struct PointLightData
{
	math::Color color;
	math::Vector3 position;
	float radius;
};

//-------------------------------------------------------------------------

class PointLightComponent : public virtual LightComponent
{
	RTTI_CLASS(PointLightComponent, LightComponent)

public:
	using Data = PointLightData;

	PointLightComponent();
	~PointLightComponent();

	Data GetData() const;

	const math::Vector3& GetPosition() const { return m_position; }
	void SetPosition(const math::Vector3& position);

	float GetRadius() const { return m_radius; }
	void SetRadius(const float radius);

	bool IsVisibleFrom(const math::Vector3& positionFrom) const;

protected:
	math::Vector3 m_position;
	float m_radius;
};
} // namespace library
