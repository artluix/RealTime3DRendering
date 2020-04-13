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

		Vector4 _vec;
	};

	constexpr Color(const float r, const float g, const float b, const float a = 1.f) : r(r), g(g), b(b), a(a) {}
	explicit constexpr Color(const float v = 0.f) : Color(v, v, v) {}

	explicit constexpr Color(const Vector4& vec) : Color(vec.x, vec.y, vec.z, vec.w) {}
	Color& operator=(const Vector4& vec) { _vec = vec; return *this; }
	explicit operator const Vector4& () const { return ToVector(); }
	const Vector4& ToVector() const { return _vec; }

	explicit Color(const dx::VECTOR& dxVEC) : _vec(dxVEC) { }
	Color& operator=(const dx::VECTOR& dxVEC) { _vec = dxVEC; return *this; }
	explicit operator dx::VECTOR() const { return static_cast<dx::VECTOR>(_vec); }

	explicit operator const float* () const { return &r; }

	bool operator==(const Color& other) const { return _vec == other._vec; }
	bool operator!=(const Color& other) const { return _vec != other._vec; }
	bool operator>(const Color& other) const { return _vec > other._vec; }
	bool operator>=(const Color& other) const { return _vec >= other._vec; }
	bool operator<(const Color& other) const { return _vec < other._vec; }
	bool operator<=(const Color& other) const { return _vec <= other._vec; }

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

inline constexpr Color Red = Color::FromBytes(255, 0, 0);
inline constexpr Color Green = Color::FromBytes(255, 0, 0);
inline constexpr Color Blue = Color::FromBytes(255, 0, 0);

inline constexpr Color Black = Color::FromBytes(0, 0, 0);
inline constexpr Color White = Color::FromBytes(255, 255, 255);

inline constexpr Color CornFlower = Color::FromBytes(100, 149, 237);
} // namespace colors
} // namespace library

//-------------------------------------------------------------------------

namespace dx::math
{
template<>
constexpr inline auto DXGI_FORMAT<library::math::Color> = DXGI_FORMAT_R32G32B32A32_FLOAT;
} // namespace dx::math

