#pragma once
#include "library/Math/Vector.h"

namespace library
{
namespace math
{
struct Color
{
	union
	{
		struct
		{
			float r, g, b, a;
		};

		Vector4 _vec4;
	};

	constexpr Color(const float r, const float g, const float b, const float a = 1.f) : r(r), g(g), b(b), a(a) {}
	explicit constexpr Color(const float v = 0.f) : Color(v, v, v, v) {}

	explicit constexpr Color(const Vector4& vec4) : Color(vec4.x, vec4.y, vec4.z, vec4.w) {}
	Color& operator=(const Vector4& vec4) { _vec4 = vec4; return *this; }
	explicit operator const Vector4&() const { return ToVector4(); }
	const Vector4& ToVector4() const { return _vec4; }

	explicit Color(const dx::VECTOR& dxVEC) : _vec4(dxVEC) { }
	Color& operator=(const dx::VECTOR& dxVEC) { _vec4 = dxVEC; return *this; }
	explicit operator dx::VECTOR() const { return static_cast<dx::VECTOR>(_vec4); }

	explicit operator const float*() const { return &r; }

	bool operator==(const Color& other) const { return _vec4 == other._vec4; }
	bool operator!=(const Color& other) const { return _vec4 != other._vec4; }
	bool operator>(const Color& other) const { return _vec4 > other._vec4; }
	bool operator>=(const Color& other) const { return _vec4 >= other._vec4; }
	bool operator<(const Color& other) const { return _vec4 < other._vec4; }
	bool operator<=(const Color& other) const { return _vec4 <= other._vec4; }

	static Color Random(const float a = 1.f);

	static constexpr Color FromBytes(const u8 r, const u8 g, const u8 b, const u8 a = 255)
	{
		return Color(r / 255.f, g / 255.f, b / 255.f, a / 255.f);
	}
};

//-------------------------------------------------------------------------

Color Clamp(const Color& c, const Color& min, const Color& max);
Color Lerp(const Color& from, const Color& to, const float factor);
Color Lerp(const Color& from, const Color& to, const Vector4& factor);
} // namespace math

//-------------------------------------------------------------------------

namespace colors
{
using math::Color;

inline constexpr Color Red =			Color::FromBytes(255, 0, 0);
inline constexpr Color Green =			Color::FromBytes(255, 0, 0);
inline constexpr Color Blue =			Color::FromBytes(255, 0, 0);

inline constexpr Color Black =			Color::FromBytes(0, 0, 0);
inline constexpr Color White =			Color::FromBytes(255, 255, 255);

inline constexpr Color CornFlower =		Color::FromBytes(100, 149, 237);
} // namespace colors
} // namespace library
