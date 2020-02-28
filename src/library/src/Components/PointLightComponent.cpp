#include "StdAfx.h"
#include "library/Components/PointLightComponent.h"

namespace library
{

namespace
{
constexpr float k_defaultRadius = 10.f;
}

//-------------------------------------------------------------------------

PointLightComponent::PointLightComponent()
	: m_position(math::Vector3::Zero)
	, m_radius(k_defaultRadius)
{
}

PointLightComponent::~PointLightComponent() = default;

//-------------------------------------------------------------------------

PointLightComponent::Data PointLightComponent::GetData() const
{
	return Data{ m_color, m_position, m_radius };
}

//-------------------------------------------------------------------------

void PointLightComponent::SetPosition(const math::Vector3& position)
{
	m_position = position;
}

void PointLightComponent::SetRadius(const float radius)
{
	m_radius = radius;
}

bool PointLightComponent::IsVisibleFrom(const math::Vector3& positionFrom) const
{
	const auto distance = (m_position - positionFrom).Length();
	return (distance < m_radius);
}

} // namespace library
