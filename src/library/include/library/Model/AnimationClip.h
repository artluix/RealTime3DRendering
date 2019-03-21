#pragma once
#include "library/NonCopyable.hpp"
#include "library/Time.h"

#include <string>
#include <map>
#include <vector>

struct aiAnimation;

namespace library
{
	class BoneAnimation;

	class AnimationClip : public NonCopyable<AnimationClip>
	{
		friend class Model;

	public:
		~AnimationClip();

		const std::string& GetName() const { return m_name; }
		const Duration& GetDuration() const { return m_duration; }
		float GetTicksPerSecond() const { return m_ticksPerSecond; }
		unsigned GetKeyframesCount() const { return m_keyframesCount; }

	private:
		using BoneAnimationPtr = std::unique_ptr<BoneAnimation>;

		explicit AnimationClip(const Model& model, const aiAnimation& aiAnimation);

		std::string m_name;
		Duration m_duration;
		float m_ticksPerSecond;

		std::vector<BoneAnimationPtr> m_boneAnimations;
		std::map<std::string, unsigned> m_boneAnimationsIndexMapping;
		unsigned m_keyframesCount;
	};
} // namespace library
