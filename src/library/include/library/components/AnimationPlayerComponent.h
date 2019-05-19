#pragma once
#include "library/Components/Component.h"
#include "library/Math/Matrix.h"
#include "library/Model/AnimationDef.h"

#include <memory>

namespace library
{
class Model;
class AnimationClip;
class SceneNode;

class AnimationPlayerComponent : public virtual Component
{
	RTTI_CLASS(AnimationPlayerComponent, Component)

public:
	using Duration = animation::Duration;
	using TimePoint = animation::TimePoint;

	explicit AnimationPlayerComponent(const Model& model, const bool enableInterpolation = true);

	const Model& GetModel() const { return m_model; }
	const AnimationClip* GetAnimation() const { return m_animation; }
	const Duration& GetTime() const { return m_time; }

	const math::Matrix4& GetBoneTransform(const unsigned boneIdx) const;
	const std::vector<math::Matrix4>& GetBoneTransforms() const { return m_finalTransforms; }

	bool IsPlaying() const { return m_isPlaying; }

	void SetCurrentKeyframe(const unsigned keyframe);
	unsigned GetCurrentKeyframe() const { return m_currentKeyframe; }

	void SetInterpolationEnabled(const bool enabled);
	bool IsInterpolationEnabled() const { return m_isInterpolationEnabled; }

	void SetLooped(const bool looped);
	bool IsLooped() const { return m_isLooped; }

	void Play(const AnimationClip& animation);
	void Pause();
	void Resume();

	void Update(const Time& time) override;

private:
	struct SceneNodeLess {
		bool operator()(const SceneNode& lhs, const SceneNode& rhs) const { return &lhs < &rhs; }
	};

	//-------------------------------------------------------------------------

	void GetBindPoseBottomUp(const SceneNode& sceneNode);
	void GetBindPose(const SceneNode& sceneNode, const math::Matrix4& parentToRootTransform);

	void GetPose(const SceneNode& sceneNode, const math::Matrix4& parentToRootTransform, const TimePoint& timePoint);
	void GetPose(const SceneNode& sceneNode, const math::Matrix4& parentToRootTransform, const unsigned keyframe);
	void GetInterpolatedPose(const SceneNode& sceneNode, const math::Matrix4& parentToRootTransform, const TimePoint& timePoint);

	//-------------------------------------------------------------------------
	// root scene node wrappers
	//-------------------------------------------------------------------------

	void GetBindPose();
	void GetBindPoseBottomUp();

	void GetPose(const TimePoint& timePoint);
	void GetPose(const unsigned keyframe);
	void GetInterpolatedPose(const TimePoint& timePoint);

	//-------------------------------------------------------------------------

	const Model& m_model;
	const AnimationClip* m_animation = nullptr;

	Duration m_time;
	unsigned m_currentKeyframe = 0;

	std::vector<math::Matrix4> m_finalTransforms;
	math::Matrix4 m_inverseRootTransform;

	bool m_isInterpolationEnabled;
	bool m_isPlaying = false;
	bool m_isLooped = true;
};
} // namespace library
