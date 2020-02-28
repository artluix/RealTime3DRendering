#pragma once
#include "library/NonCopyable.hpp"
#include "library/Time.h"
#include "library/Model/AnimationDef.h"
#include "library/Math/Math.h"

#include <string>
#include <map>
#include <vector>
#include <functional>
#include <memory>

struct aiAnimation;

namespace library
{
class Bone;
class BoneAnimation;

class AnimationClip : public NonCopyable<AnimationClip>
{
	friend class Model;

public:
	using TimePoint = animation::TimePoint;
	using Duration = animation::Duration;

	~AnimationClip();

	const std::string& GetName() const { return m_name; }
	const Duration& GetDuration() const { return m_duration; }
	float GetTicksPerSecond() const { return m_ticksPerSecond; }
	unsigned GetKeyframesCount() const { return unsigned(m_keyframesCount); }

	// bone animations
	bool HasBoneAnimations() const { return !m_boneAnimations.empty(); }
	unsigned GetBoneAnimationsCount() const { return unsigned(m_boneAnimations.size()); }
	bool HasBoneAnimation(const Bone& bone) const;

	unsigned GetBoneAnimationIdx(const Bone& bone) const;
	const BoneAnimation& GetBoneAnimation(const Bone& bone) const;
	// BoneAnimation& GetBoneAnimation(const Bone& bone);

	const BoneAnimation& GetBoneAnimation(const unsigned boneAnimationIdx) const;
	// BoneAnimation& GetBoneAnimation(const unsigned boneAnimationIdx);

	// transforms
	std::vector<math::Matrix4> GetTransforms(const TimePoint& timePoint) const;
	std::vector<math::Matrix4> GetTransforms(const unsigned keyframeIdx) const;
	std::vector<math::Matrix4> GetInterpolatedTransforms(const TimePoint& timePoint) const;

	// transform wrappers for BoneAnimation
	unsigned GetKeyframeIdx(const Bone& bone, const TimePoint& timePoint) const;
	const math::Matrix4& GetTransform(const Bone& bone, const TimePoint& timePoint) const;
	const math::Matrix4& GetTransform(const Bone& bone, const unsigned keyframeIdx) const;
	math::Matrix4 GetInterpolatedTransform(const Bone& bone, const TimePoint& timePoint) const;

private:
	struct BoneLess
	{
		bool operator()(const Bone& lhs, const Bone& rhs) const { return &lhs < &rhs; }
	};

	using BoneAnimationPtr = std::unique_ptr<BoneAnimation>;

	AnimationClip(const Model& model, const aiAnimation& aiAnimation);

	std::string m_name;
	Duration m_duration;
	float m_ticksPerSecond;

	std::vector<BoneAnimationPtr> m_boneAnimations;
	std::map<std::reference_wrapper<const Bone>, unsigned, BoneLess> m_boneAnimationsMapping;
	unsigned m_keyframesCount = 0;
};
} // namespace library
