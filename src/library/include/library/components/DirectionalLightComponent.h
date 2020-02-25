#pragma once
#include "library/Components/LightComponent.h"
#include "library/Math/Vector.h"
#include "library/Math/Quaternion.h"

namespace library
{
__declspec(align(16)) struct DirectionalLightData
{
	math::Color color;
	math::Vector3 direction;
};

//-------------------------------------------------------------------------

class DirectionalLightComponent : public virtual LightComponent
{
	RTTI_CLASS(DirectionalLightComponent, LightComponent)

public:
	using Data = DirectionalLightData;

	DirectionalLightComponent();
	~DirectionalLightComponent();

	Data GetData() const;

	const math::Vector3& GetDirection() const { return m_direction; }
	void SetDirection(const math::Vector3& direction);

	void Rotate(const math::Quaternion& rotation);
	void SetRotation(const math::Quaternion& rotation);

protected:
	math::Vector3 m_direction;
};
} // namespace library
