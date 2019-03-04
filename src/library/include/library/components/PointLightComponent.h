#pragma once
#include "library/Components/LightComponent.h"
#include "library/Math/Math.h"

namespace library
{
	class PointLightComponent : public virtual LightComponent
	{
	public:
		explicit PointLightComponent();
		~PointLightComponent();

		const math::Vector3& GetPosition() const { return m_position; }
		void SetPosition(const math::Vector3& position);

		float GetRadius() const { return m_radius; }
		void SetRadius(const float radius);

	protected:
		math::Vector3 m_position;
		float m_radius;
	};
} // namespace library
