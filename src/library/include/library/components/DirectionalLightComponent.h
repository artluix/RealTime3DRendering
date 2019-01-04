#pragma once
#include "library/components/LightComponent.h"
#include "library/Math.h"

namespace library
{
	class DirectionalLightComponent : public rtti::Class<DirectionalLightComponent, LightComponent>
	{
	public:
		explicit DirectionalLightComponent(const Application& app);
		~DirectionalLightComponent() = default;

		const math::Vector3& GetDirection() const { return m_direction; }
		const math::Vector3& GetUp() const { return m_up; }
		const math::Vector3& GetRight() const { return m_right; }

		void ApplyRotation(const math::Matrix4& transform);

	protected:
		math::Vector3 m_direction;
		math::Vector3 m_up;
		math::Vector3 m_right;
	};

} // namespace library
