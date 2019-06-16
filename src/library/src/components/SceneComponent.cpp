#include "StdAfx.h"
#include "library/Components/SceneComponent.h"

#include "library/Application.h"
#include "library/math/Math.h"

#include <cassert>

namespace library
{
SceneComponent::SceneComponent()
	: m_transform{}
	, m_direction(math::Direction::Forward)
	, m_up(math::Direction::Up)
	, m_right(math::Direction::Right)
{}

SceneComponent::~SceneComponent() = default;

//-------------------------------------------------------------------------

const library::math::Matrix4& SceneComponent::GetWorldMatrix() const
{
	assert(!m_isWorldMatrixDirty);
	return m_worldMatrix;
}

void SceneComponent::SetPosition(const math::Vector3& position)
{
	if (m_transform.position == position)
		return;

	const auto translationDelta = position - m_transform.position;
	m_transform.position = position;

	OnTranslation(translationDelta);

	m_isWorldMatrixDirty = true;
}

void SceneComponent::Translate(const math::Vector3& translation)
{
	if (!translation)
		return;

	m_transform.position += translation;

	OnTranslation(translation);

	m_isWorldMatrixDirty = true;
}

//-------------------------------------------------------------------------

void SceneComponent::SetRotation(const math::Quaternion& rotation)
{
	if (m_transform.rotation == rotation)
		return;
	
	const auto rotationDelta = m_transform.rotation.Inverse() * rotation;
	m_transform.rotation = rotation;

	OnRotation(rotationDelta);

	m_isWorldMatrixDirty = true;
}

void SceneComponent::Rotate(const math::Quaternion& rotation)
{
	if (rotation == math::Quaternion::Identity)
		return;

	m_transform.rotation *= rotation;

	OnRotation(rotation);

	m_isWorldMatrixDirty = true;
}

//-------------------------------------------------------------------------

void SceneComponent::SetRotation(const math::Vector3& pitchYawRoll)
{
	SetRotation(math::Quaternion::RotationPitchYawRoll(pitchYawRoll));
}

void SceneComponent::Rotate(const math::Vector3& pitchYawRoll)
{
	Rotate(math::Quaternion::RotationPitchYawRoll(pitchYawRoll));
}

//-------------------------------------------------------------------------

void SceneComponent::SetScaling(const math::Vector3& scaling)
{
	if (scaling < math::Vector3::Zero)
		return;

	if (m_transform.scaling == scaling)
		return;

	const auto scalingDelta = scaling - m_transform.scaling;
	m_transform.scaling = scaling;

	OnScaling(scalingDelta);

	m_isWorldMatrixDirty = true;
}

void SceneComponent::Scale(const math::Vector3& scaling)
{
	if (!scaling)
		return;

	m_transform.scaling += scaling;

	OnScaling(scaling);

	m_isWorldMatrixDirty = true;
}

//-------------------------------------------------------------------------

void SceneComponent::SetInitialTransform(const math::Matrix4& initialTransform)
{
	m_initialTransformMatrix = initialTransform;
	m_isWorldMatrixDirty = true;
}

//-------------------------------------------------------------------------

void SceneComponent::UpdateWorldMatrix()
{
	if (!m_isWorldMatrixDirty)
		return;

	const auto scalingMatrix = math::Matrix4::Scaling(m_transform.scaling);
	const auto translationMatrix = math::Matrix4::Translation(m_transform.position);
	const auto rotationMatrix = math::Matrix4::RotationQuaternion(m_transform.rotation);

	m_direction = rotationMatrix.GetForward();
	m_up = rotationMatrix.GetUp();
	m_right = rotationMatrix.GetRight();

	m_worldMatrix = m_initialTransformMatrix * (scalingMatrix * rotationMatrix * translationMatrix);

	m_isWorldMatrixDirty = false;
}

void SceneComponent::Update(const Time& time)
{
	UpdateWorldMatrix();
}
} // namespace library
