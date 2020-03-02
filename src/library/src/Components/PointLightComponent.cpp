#include "StdAfx.h"
#include "library/Components/PointLightComponent.h"

#ifdef USE_LIGHT_PROXY_MODEL
#include "library/Components/ProxyModelComponent.h"
#endif

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

void PointLightComponent::SetPosition(const math::Vector3& position)
{
	m_position = position;
#ifdef USE_LIGHT_PROXY_MODEL
	if (!!m_proxyModel)
		m_proxyModel->SetPosition(position);
#endif
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

#ifdef USE_LIGHT_PROXY_MODEL
void PointLightComponent::SetupProxyModel(const CameraComponent& camera)
{
	m_proxyModel = std::make_unique<ProxyModelComponent>("PointLightProxy", 0.5f);
	m_proxyModel->SetCamera(camera);
	m_proxyModel->Initialize(GetApp());
}
#endif

} // namespace library
