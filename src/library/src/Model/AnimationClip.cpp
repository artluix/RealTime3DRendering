#include "StdAfx.h"
#include "library/Model/AnimationClip.h"

#include "library/Model/Model.h"
#include "library/Model/BoneAnimation.h"

#include <assimp/scene.h>
#include <cassert>

namespace library
{
AnimationClip::AnimationClip(const Model& model, const aiAnimation& aiAnimation)
	: m_name(aiAnimation.mName.C_Str())
	, m_duration(float(aiAnimation.mDuration))
	, m_ticksPerSecond(float(aiAnimation.mTicksPerSecond))
{
	assert(aiAnimation.mNumChannels > 0);

	m_ticksPerSecond = math::Max(0.f, m_ticksPerSecond);

	m_boneAnimations.reserve(aiAnimation.mNumChannels);

	for (unsigned i = 0; i < aiAnimation.mNumChannels; i++)
	{
		const auto& boneAnimation = m_boneAnimations.emplace_back(
			BoneAnimationPtr(new BoneAnimation(model, *aiAnimation.mChannels[i])));
		assert(!HasBoneAnimation(boneAnimation->GetBone()));
		m_boneAnimationsMapping.emplace(boneAnimation->GetBone(), i);
	}

	for (const auto& boneAnimation : m_boneAnimations)
	{
		m_keyframesCount = math::Max(m_keyframesCount, boneAnimation->GetKeyframesCount());
	}
}

AnimationClip::~AnimationClip() = default;

//-------------------------------------------------------------------------
// bone animations
//-------------------------------------------------------------------------

bool AnimationClip::HasBoneAnimation(const Bone& bone) const
{
	return m_boneAnimationsMapping.find(bone) != m_boneAnimationsMapping.end();
}

unsigned AnimationClip::GetBoneAnimationIdx(const Bone& bone) const
{
	assert(HasBoneAnimation(bone));
	return m_boneAnimationsMapping.at(bone);
}

const BoneAnimation& AnimationClip::GetBoneAnimation(const Bone& bone) const
{
	return GetBoneAnimation(GetBoneAnimationIdx(bone));
}

// BoneAnimation& AnimationClip::GetBoneAnimation(const Bone& bone)
//{
//	return GetBoneAnimation(GetBoneAnimationIdx(bone));
//}

const BoneAnimation& AnimationClip::GetBoneAnimation(const unsigned boneAnimationIdx) const
{
	assert(boneAnimationIdx < m_boneAnimations.size());
	return *m_boneAnimations.at(boneAnimationIdx);
}

// BoneAnimation& AnimationClip::GetBoneAnimation(const unsigned boneAnimationIdx)
//{
//	assert(boneAnimationIdx < m_boneAnimations.size());
//	return *m_boneAnimations.at(boneAnimationIdx);
//}

//-------------------------------------------------------------------------
// transforms
//-------------------------------------------------------------------------

DynArray<math::Matrix4> AnimationClip::GetTransforms(const TimePoint& timePoint) const
{
	DynArray<math::Matrix4> transforms;
	transforms.reserve(m_boneAnimations.size());

	for (const auto& boneAnimation : m_boneAnimations)
	{
		transforms.push_back(boneAnimation->GetTransform(timePoint));
	}

	return transforms;
}

DynArray<math::Matrix4> AnimationClip::GetTransforms(const unsigned keyframeIdx) const
{
	DynArray<math::Matrix4> transforms;
	transforms.reserve(m_boneAnimations.size());

	for (const auto& boneAnimation : m_boneAnimations)
	{
		transforms.push_back(boneAnimation->GetTransform(keyframeIdx));
	}

	return transforms;
}

DynArray<math::Matrix4> AnimationClip::GetInterpolatedTransforms(const TimePoint& timePoint) const
{
	DynArray<math::Matrix4> transforms;
	transforms.reserve(m_boneAnimations.size());

	for (const auto& boneAnimation : m_boneAnimations)
	{
		transforms.push_back(boneAnimation->GetInterpolatedTransform(timePoint));
	}

	return transforms;
}

//-------------------------------------------------------------------------

unsigned AnimationClip::GetKeyframeIdx(const Bone& bone, const TimePoint& timePoint) const
{
	if (HasBoneAnimation(bone))
		return GetBoneAnimation(bone).GetKeyframeIdx(timePoint);

	return unsigned(-1);
}

const math::Matrix4& AnimationClip::GetTransform(const Bone& bone, const TimePoint& timePoint) const
{
	if (HasBoneAnimation(bone))
		return GetBoneAnimation(bone).GetTransform(timePoint);

	return math::Matrix4::Identity;
}

const math::Matrix4& AnimationClip::GetTransform(const Bone& bone, const unsigned keyframeIdx) const
{
	if (HasBoneAnimation(bone))
		return GetBoneAnimation(bone).GetTransform(keyframeIdx);

	return math::Matrix4::Identity;
}

math::Matrix4 AnimationClip::GetInterpolatedTransform(const Bone& bone, const TimePoint& timePoint) const
{
	if (HasBoneAnimation(bone))
		return GetBoneAnimation(bone).GetInterpolatedTransform(timePoint);

	return math::Matrix4::Identity;
}
} // namespace library
