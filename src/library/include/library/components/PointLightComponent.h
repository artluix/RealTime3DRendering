#pragma once
#include "library/components/LightComponent.h"
#include "library/math/Math.h"

namespace library
{
	namespace components
	{
		class PointLight : public virtual Light
		{
		public:
			explicit PointLight();
			~PointLight() = default;

			const math::Vector3& GetPosition() const { return m_position; }
			void SetPosition(const math::Vector3& position);

			float GetRadius() const { return m_radius; }
			void SetRadius(const float radius);

		protected:
			math::Vector3 m_position;
			float m_radius;
		};

	} // namespace components
} // namespace library
