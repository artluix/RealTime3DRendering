#pragma once
#include "library/Math.h"

namespace library
{
	class Color : public math::Vector4
	{
	public:
		Color() = default;
		constexpr Color(const float r, const float g, const float b, const float a) : math::Vector4(r, g, b, a) {}
		Color(const DirectX::XMFLOAT4& xmFloat4) : math::Vector4(xmFloat4) {}

		void Transform(const math::Matrix4&) = delete;
		math::Vector4 Cross(const math::Vector4&) = delete;
	};
	
	namespace colors
	{
		inline constexpr Color White = Color(1.f, 1.f, 1.f, 1.f);
		inline constexpr Color Black = Color(0.f, 0.f, 0.f, 1.f);
		inline constexpr Color Red = Color(1.f, 0.f, 0.f, 1.f);
		inline constexpr Color Green = Color(0.f, 1.f, 0.f, 1.f);
		inline constexpr Color Blue = Color(0.f, 0.f, 1.f, 1.f);
		inline constexpr Color Cyan = Color(0.f, 1.f, 1.f, 1.f);
		inline constexpr Color Magenta = Color(1.f, 0.f, 1.f, 1.f);
		inline constexpr Color Yellow = Color(1.f, 1.f, 0.f, 1.f);
	} // namespace colors
} // namespace library
