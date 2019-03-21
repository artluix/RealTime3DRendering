#include "StdAfx.h"
#include "library/Model/Keyframe.h"

namespace library
{
	Keyframe::Keyframe(
		const TimePoint& timePoint,
		const math::Vector3& translation,
		const math::Vector4& rotationQuat,
		const math::Vector3& scale
	)
		: m_timePoint(timePoint)

		, m_translation(translation)
		, m_rotationQuat(rotationQuat)
		, m_scale(scale)
	{
		m_transform = math::Matrix4::AffineTransformation(
			m_scale, math::Vector4::Zero, m_rotationQuat, m_translation
		);
	}

	Keyframe::Keyframe(
		const float time,
		const math::Vector3& translation,
		const math::Vector4& rotationQuat,
		const math::Vector3& scale
	)
		: Keyframe(TimePoint(animation::Duration(time)), translation, rotationQuat, scale)
	{
	}
} // namespace library
