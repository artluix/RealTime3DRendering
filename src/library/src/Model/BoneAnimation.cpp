#include "StdAfx.h"
#include "library/Model/BoneAnimation.h"

#include "library/Model/Model.h"
#include "library/Model/Keyframe.h"

#include <assimp/scene.h>
#include <cassert>
#include <algorithm>

namespace library
{
BoneAnimation::BoneAnimation(const Model& model, const aiNodeAnim& aiNodeAnim)
	: m_model(model)
	, m_bone(model.GetBone(aiNodeAnim.mNodeName.C_Str()))
{
	assert(aiNodeAnim.mNumPositionKeys == aiNodeAnim.mNumRotationKeys);
	assert(aiNodeAnim.mNumPositionKeys == aiNodeAnim.mNumScalingKeys);

	m_keyframes.reserve(aiNodeAnim.mNumPositionKeys);

	for (unsigned i = 0; i < aiNodeAnim.mNumPositionKeys; i++)
	{
		const auto& positionKey = aiNodeAnim.mPositionKeys[i];
		const auto& rotationKey = aiNodeAnim.mRotationKeys[i];
		const auto& scaleKey = aiNodeAnim.mScalingKeys[i];

		assert(positionKey.mTime == rotationKey.mTime);
		assert(positionKey.mTime == scaleKey.mTime);

		const auto time = static_cast<float>(positionKey.mTime);

		const auto position = reinterpret_cast<const math::Vector3&>(positionKey.mValue);

		// assimp stores quaternions differently
		const auto rotation = math::Quaternion(
			rotationKey.mValue.x,
			rotationKey.mValue.y,
			rotationKey.mValue.z,
			rotationKey.mValue.w);

		const auto scale = reinterpret_cast<const math::Vector3&>(scaleKey.mValue);

		m_keyframes.emplace_back(KeyframePtr(new Keyframe(time, position, rotation, scale)));
	}
}

BoneAnimation::~BoneAnimation() = default;

//-------------------------------------------------------------------------

const Keyframe& BoneAnimation::GetKeyframe(const std::size_t keyframeIdx) const
{
	return *m_keyframes[math::Min(keyframeIdx, m_keyframes.size() - 1)];
}

unsigned BoneAnimation::GetKeyframeIdx(const TimePoint& timePoint) const
{
	if (timePoint <= m_keyframes.front()->GetTimePoint())
		return 0U;

	if (timePoint >= m_keyframes.back()->GetTimePoint())
		return GetKeyframesCount() - 1;

	auto findPred = [](const KeyframePtr& lhs, const KeyframePtr& rhs) -> bool {
		return lhs->GetTimePoint() < rhs->GetTimePoint();
	};

	const auto keyframeToFind =
		KeyframePtr(new Keyframe(
			timePoint,
			math::Vector3::Zero,
			math::Quaternion::Zero,
			math::Vector3::Zero
		));

	const auto it = std::
		upper_bound(std::next(m_keyframes.cbegin()), std::prev(m_keyframes.cend()), keyframeToFind, findPred);

	// std::distance return index, but we need -1
	return static_cast<unsigned>(std::distance(m_keyframes.cbegin(), it)) - 1;
}

const math::Matrix4& BoneAnimation::GetTransform(const std::size_t keyframeIdx) const
{
	return m_keyframes[math::Min(keyframeIdx, m_keyframes.size() - 1)]->GetTransform();
}

const math::Matrix4& BoneAnimation::GetTransform(const TimePoint& timePoint) const
{
	return GetTransform(GetKeyframeIdx(timePoint));
}

math::Matrix4 BoneAnimation::GetInterpolatedTransform(const TimePoint& timePoint) const
{
	// time before start of animation
	if (timePoint <= m_keyframes.front()->GetTimePoint())
		return m_keyframes.front()->GetTransform();

	// time after end time of animation
	if (timePoint >= m_keyframes.back()->GetTimePoint())
		return m_keyframes.back()->GetTransform();

	// interpolate between 2 keyframes
	const auto keyframeIdx = GetKeyframeIdx(timePoint);
	const auto& leftKeyframe = *m_keyframes[keyframeIdx];
	const auto& rightKeyframe = *m_keyframes[keyframeIdx + 1];

	const auto lerpFactor = (timePoint - leftKeyframe.GetTimePoint()) /
							(rightKeyframe.GetTimePoint() - leftKeyframe.GetTimePoint());

	const auto translation =
		math::Lerp(leftKeyframe.GetTranslation(), rightKeyframe.GetTranslation(), lerpFactor);

	const auto rotationQuat =
		math::Lerp(leftKeyframe.GetRotationQuat(), rightKeyframe.GetRotationQuat(), lerpFactor);

	const auto scale = math::Lerp(leftKeyframe.GetScale(), rightKeyframe.GetScale(), lerpFactor);
	return math::Matrix4::AffineTransformation(scale, math::Vector4::Zero, rotationQuat, translation);
}
} // namespace library
