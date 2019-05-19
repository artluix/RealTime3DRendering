#pragma once
#include "library/NonCopyable.hpp"
#include "library/Model/AnimationDef.h"
#include "library/Math/Matrix.h"

#include <vector>
#include <memory>

struct aiNodeAnim;

namespace library
{
class Model;
class Bone;
class Keyframe;

class BoneAnimation : public NonCopyable<BoneAnimation>
{
public:
	using TimePoint = animation::TimePoint;

	BoneAnimation(const Model& model, const aiNodeAnim& aiNodeAnim);
	~BoneAnimation();

	const Model& GetModel() const { return m_model; }
	const Bone& GetBone() const { return m_bone; }

	unsigned GetKeyframesCount() const { return static_cast<unsigned>(m_keyframes.size()); }
	const Keyframe& GetKeyframe(const std::size_t keyframeIdx) const;

	unsigned GetKeyframeIdx(const TimePoint& timePoint) const;
	const math::Matrix4& GetTransform(const TimePoint& timePoint) const;
	const math::Matrix4& GetTransform(const std::size_t keyframeIdx) const;
	math::Matrix4 GetInterpolatedTransform(const TimePoint& timePoint) const;

private:
	using KeyframePtr = std::unique_ptr<Keyframe>;

	const Model& m_model;
	const Bone& m_bone;
	std::vector<KeyframePtr> m_keyframes;
};
} // namespace library
