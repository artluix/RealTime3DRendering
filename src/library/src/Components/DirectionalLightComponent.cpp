#include "StdAfx.h"
#include "library/Components/DirectionalLightComponent.h"

#include "library/Math/Direction.h"
#include "library/Math/Matrix.h"
#include "library/Math/MathConstants.h"

#ifdef USE_LIGHT_PROXY_MODEL
#include "library/Components/ProxyModelComponent.h"
#include "library/Components/CameraComponent.h"
#endif

namespace library
{

namespace
{
constexpr auto k_defaultDirection = math::Direction::Forward;
constexpr auto k_proxyModelRotationOffset = math::Vector3(0.f, math::Pi_Div_2, 0.f);
}

DirectionalLightComponent::DirectionalLightComponent()
	: m_direction(k_defaultDirection)
{
}

DirectionalLightComponent::~DirectionalLightComponent() = default;

//-------------------------------------------------------------------------

void DirectionalLightComponent::SetDirection(const math::Vector3& direction)
{
	m_direction = direction;
}

//-------------------------------------------------------------------------

void DirectionalLightComponent::Rotate(const math::Quaternion& rotation)
{
	if (rotation == math::Quaternion::Identity)
		return;

	const auto rotationMatrix = math::Matrix4::RotationQuaternion(rotation);
	m_direction = m_direction.Transform(rotationMatrix);
#ifdef USE_LIGHT_PROXY_MODEL
	if (!!m_proxyModel)
		m_proxyModel->Rotate(rotation);
#endif
}

void DirectionalLightComponent::SetRotation(const math::Quaternion& rotation)
{
	const auto rotationMatrix = math::Matrix4::RotationQuaternion(rotation);
	m_direction = rotationMatrix.GetForward();
#ifdef USE_LIGHT_PROXY_MODEL
	if (!!m_proxyModel)
		m_proxyModel->SetRotation(rotation);
#endif
}

#ifdef USE_LIGHT_PROXY_MODEL
void DirectionalLightComponent::SetupProxyModel(const CameraComponent& camera)
{
	m_proxyModel = std::make_unique<ProxyModelComponent>("DirectionalLightProxy", 0.2f);
	m_proxyModel->SetInitialTransform(math::Matrix4::RotationPitchYawRoll(k_proxyModelRotationOffset));
	m_proxyModel->SetCamera(camera);
	m_proxyModel->Initialize(camera.GetApp());
}
#endif

} // namespace library
