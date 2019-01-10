#pragma once
#include "library/math/Vector.h"

namespace library
{
	class Color : public Vector4
	{
	public:
		Color() = default;
		constexpr Color(const float r, const float g, const float b, const float a) : Vector4(r, g, b, a) {}
		Color(const DirectX::XMFLOAT4& xmFloat4) : Vector4(xmFloat4) {}

		void Transform(const Matrix4&) = delete;
		Vector4 Cross(const Vector4&) = delete;

		static const Color White;
		static const Color Black;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Cyan;
		static const Color Magenta;
		static const Color Yellow;

		static Color Random();
	};
} // namespace library
