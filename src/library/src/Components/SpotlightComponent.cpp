#include "StdAfx.h"
#include "library/Components/SpotlightComponent.h"

#include "library/Math/MathConstants.h"

#ifdef USE_LIGHT_PROXY_MODEL
#include "library/Components/ProxyModelComponent.h"
#include "library/Components/CameraComponent.h"
#endif

namespace library
{

namespace
{
constexpr float k_defaultInnerAngle = 0.75f;
constexpr float k_defaultOuterAngle = 0.25f;

constexpr auto k_proxyModelRotationOffset = math::Vector3(math::Pi_Div_2, 0.f, 0.f);
} // namespace

//-------------------------------------------------------------------------

SpotlightComponent::SpotlightComponent()
	//: LightComponent()
	: m_innerAngle(k_defaultInnerAngle)
	, m_outerAngle(k_defaultOuterAngle)
{
}

SpotlightComponent::~SpotlightComponent() = default;

//-------------------------------------------------------------------------

void SpotlightComponent::SetInnerAngle(const float innerAngle)
{
	m_innerAngle = innerAngle;
}

void SpotlightComponent::SetOuterAngle(const float outerAngle)
{
	m_outerAngle = outerAngle;
}

#ifdef USE_LIGHT_PROXY_MODEL
void SpotlightComponent::SetupProxyModel(const CameraComponent& camera)
{
	m_proxyModel = std::make_unique<ProxyModelComponent>("SpotlightProxy", 0.3f);
	m_proxyModel->SetInitialTransform(math::Matrix4::RotationPitchYawRoll(k_proxyModelRotationOffset));
	m_proxyModel->SetCamera(camera);
	m_proxyModel->Initialize(camera.GetApp());
}
#endif

} // namespace library
