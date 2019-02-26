#pragma once
#include "library/math/Vector.h"

namespace library
{
	class Color : public math::Vector4
	{
	public:
		Color() = default;
		constexpr Color(
			const float r,
			const float g,
			const float b,
			const float a
		)
			: math::Vector4(r, g, b, a)
		{}

		Color(const DirectX::XMFLOAT4& xmFloat4) : math::Vector4(xmFloat4) {}

		void Transform(const math::Matrix4&) = delete;
		math::Vector4 Cross(const math::Vector4&) = delete;
		math::Vector4 Dot(const math::Vector4&) = delete;
		float LengthSq() = delete;

		static const Color White;
		static const Color Black;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Cyan;
		static const Color Magenta;
		static const Color Yellow;
		static const Color CornFlower;

		static Color Random();
	};
} // namespace library
