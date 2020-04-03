#pragma once
#include "library/NonCopyable.h"
#include "library/Math/Math.h"
#include "library/Model/AnimationDef.h"

namespace library
{
class Keyframe : public NonCopyable<Keyframe>
{
	friend class BoneAnimation;

public:
	using TimePoint = animation::TimePoint;

	const TimePoint& GetTimePoint() const { return m_timePoint; }

	const math::Vector3& GetTranslation() const { return m_translation; }
	const math::Quaternion& GetRotationQuat() const { return m_rotationQuat; }
	const math::Vector3& GetScale() const { return m_scale; }

	const math::Matrix4& GetTransform() const { return m_transform; }

private:
	Keyframe(
		const TimePoint& timePoint,
		const math::Vector3& translation,
		const math::Quaternion& rotationQuat,
		const math::Vector3& scale
	);
	Keyframe(
		const float time,
		const math::Vector3& translation,
		const math::Quaternion& rotationQuat,
		const math::Vector3& scale
	);

	TimePoint m_timePoint;

	math::Vector3 m_translation;
	math::Quaternion m_rotationQuat;
	math::Vector3 m_scale;

	math::Matrix4 m_transform;
};
} // namespace library
