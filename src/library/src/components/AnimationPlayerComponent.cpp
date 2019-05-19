#include "StdAfx.h"
#include "library/Components/AnimationPlayerComponent.h"

#include "library/Model/Bone.h"
#include "library/Model/BoneAnimation.h"
#include "library/Model/Model.h"
#include "library/Model/AnimationClip.h"

#include <cassert>

namespace library
{
AnimationPlayerComponent::AnimationPlayerComponent(
	const Model& model,
	const bool enableInterpolation /*= true*/
	)
	: m_model(model)
	, m_isInterpolationEnabled(enableInterpolation)
	, m_inverseRootTransform(math::Matrix4::Identity)
	, m_time(Duration::zero())
{
	m_finalTransforms.resize(model.GetBonesCount(), math::Matrix4::Identity);
}

//-------------------------------------------------------------------------

const math::Matrix4& AnimationPlayerComponent::GetBoneTransform(const unsigned boneIdx) const
{
	assert(boneIdx < m_finalTransforms.size());
	return m_finalTransforms[boneIdx];
}

//-------------------------------------------------------------------------

void AnimationPlayerComponent::SetCurrentKeyframe(const unsigned keyframe)
{
	m_currentKeyframe = keyframe;
	GetPose(keyframe);
}

void AnimationPlayerComponent::SetInterpolationEnabled(const bool enabled)
{
	m_isInterpolationEnabled = enabled;
}

void AnimationPlayerComponent::SetLooped(const bool looped)
{
	m_isLooped = looped;
}

//-------------------------------------------------------------------------

void AnimationPlayerComponent::Play(const AnimationClip& animation)
{
	m_animation = &animation;
	m_time = Duration::zero();
	m_currentKeyframe = 0;
	m_isPlaying = true;

	m_inverseRootTransform = m_model.GetRootNode()->GetTransform().Inverse();
	GetBindPose();
}

void AnimationPlayerComponent::Pause()
{
	m_isPlaying = false;
}

void AnimationPlayerComponent::Resume()
{
	if (!!m_animation)
		m_isPlaying = true;
}

//-------------------------------------------------------------------------

void AnimationPlayerComponent::Update(const Time& time)
{
	if (!m_isPlaying)
		return;

	assert(!!m_animation);

	m_time +=
		std::chrono::duration_cast<Duration>(time.elapsed.GetDuration()) * m_animation->GetTicksPerSecond();

	if (m_time >= m_animation->GetDuration())
	{
		if (m_isLooped)
		{
			m_time = Duration::zero();
		}
		else
		{
			m_isPlaying = false;
			return;
		}
	}

	const auto timePoint = TimePoint(m_time);

	if (m_isInterpolationEnabled)
		GetInterpolatedPose(timePoint);
	else
		GetPose(timePoint);
}

//-------------------------------------------------------------------------

void AnimationPlayerComponent::GetBindPoseBottomUp(const SceneNode& sceneNode)
{
	auto toRootTransform = sceneNode.GetTransform();

	for (auto parentNode = sceneNode.GetParent(); !!parentNode; parentNode = parentNode->GetParent())
	{
		toRootTransform *= parentNode->GetTransform();
	}

	if (const auto bone = sceneNode.As<Bone>())
	{
		m_finalTransforms[bone->GetIndex()] =
			bone->GetOffsetTransform() * toRootTransform * m_inverseRootTransform;
	}

	for (unsigned i = 0, childrenCount = sceneNode.GetChildrenCount(); i < childrenCount; i++)
	{
		GetBindPoseBottomUp(sceneNode.GetChild(i));
	}
}

void AnimationPlayerComponent::GetBindPose(
	const SceneNode& sceneNode,
	const math::Matrix4& parentToRootTransform)
{
	const auto toRootTransform = sceneNode.GetTransform() * parentToRootTransform;

	if (const auto bone = sceneNode.As<Bone>())
	{
		m_finalTransforms[bone->GetIndex()] =
			bone->GetOffsetTransform() * toRootTransform * m_inverseRootTransform;
	}

	for (unsigned i = 0, childrenCount = sceneNode.GetChildrenCount(); i < childrenCount; i++)
	{
		GetBindPose(sceneNode.GetChild(i), toRootTransform);
	}
}

void AnimationPlayerComponent::GetPose(
	const SceneNode& sceneNode,
	const math::Matrix4& parentToRootTransform,
	const TimePoint& timePoint)
{
	const auto bone = sceneNode.As<Bone>();

	const auto& toParentTransform =
		!!bone ? m_animation->GetTransform(*bone, timePoint) : sceneNode.GetTransform();

	const auto toRootTransform = toParentTransform * parentToRootTransform;

	if (!!bone)
	{
		m_finalTransforms[bone->GetIndex()] =
			bone->GetOffsetTransform() * toRootTransform * m_inverseRootTransform;
	}

	for (unsigned i = 0, childrenCount = sceneNode.GetChildrenCount(); i < childrenCount; i++)
	{
		GetPose(sceneNode.GetChild(i), toRootTransform, timePoint);
	}
}

void AnimationPlayerComponent::GetPose(
	const SceneNode& sceneNode,
	const math::Matrix4& parentToRootTransform,
	const unsigned keyframe)
{
	const auto bone = sceneNode.As<Bone>();

	const auto& toParentTransform =
		!!bone ? m_animation->GetTransform(*bone, keyframe) : sceneNode.GetTransform();

	const auto toRootTransform = toParentTransform * parentToRootTransform;

	if (!!bone)
	{
		m_finalTransforms[bone->GetIndex()] =
			bone->GetOffsetTransform() * toRootTransform * m_inverseRootTransform;
	}

	for (unsigned i = 0, childrenCount = sceneNode.GetChildrenCount(); i < childrenCount; i++)
	{
		GetPose(sceneNode.GetChild(i), toRootTransform, keyframe);
	}
}

void AnimationPlayerComponent::GetInterpolatedPose(
	const SceneNode& sceneNode,
	const math::Matrix4& parentToRootTransform,
	const TimePoint& timePoint)
{
	const auto bone = sceneNode.As<Bone>();

	const auto& toParentTransform =
		!!bone ? m_animation->GetInterpolatedTransform(*bone, timePoint) : sceneNode.GetTransform();

	const auto toRootTransform = toParentTransform * parentToRootTransform;

	if (!!bone)
	{
		m_finalTransforms[bone->GetIndex()] =
			bone->GetOffsetTransform() * toRootTransform * m_inverseRootTransform;
	}

	for (unsigned i = 0, childrenCount = sceneNode.GetChildrenCount(); i < childrenCount; i++)
	{
		GetInterpolatedPose(sceneNode.GetChild(i), toRootTransform, timePoint);
	}
}

//-------------------------------------------------------------------------

void AnimationPlayerComponent::GetBindPoseBottomUp()
{
	GetBindPoseBottomUp(*m_model.GetRootNode());
}

void AnimationPlayerComponent::GetBindPose()
{
	GetBindPose(*m_model.GetRootNode(), math::Matrix4::Identity);
}

void AnimationPlayerComponent::GetPose(const TimePoint& timePoint)
{
	GetPose(*m_model.GetRootNode(), math::Matrix4::Identity, timePoint);
}

void AnimationPlayerComponent::GetPose(const unsigned keyframe)
{
	GetPose(*m_model.GetRootNode(), math::Matrix4::Identity, keyframe);
}

void AnimationPlayerComponent::GetInterpolatedPose(const TimePoint& timePoint)
{
	GetInterpolatedPose(*m_model.GetRootNode(), math::Matrix4::Identity, timePoint);
}
} // namespace library
