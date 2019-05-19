#pragma once
#include "library/Math/Vector.h"

namespace library::math
{
struct Color
{
	union
	{
		struct
		{
			float r, g, b, a;
		};

		std::array<float, 4> _data;
	};

	//-------------------------------------------------------------------------

	constexpr Color(const float r, const float g, const float b, const float a) : r(r), g(g), b(b), a(a) {}
	explicit constexpr Color(const float v = 0.f) : Color(v, v, v, v) {}
	explicit constexpr Color(const XMVector4& xmFloat4)
		: Color(xmFloat4.x, xmFloat4.y, xmFloat4.z, xmFloat4.w)
	{}
	explicit constexpr Color(const XMVector& xmVector)
		: Color(xmVector.m128_f32[0], xmVector.m128_f32[1], xmVector.m128_f32[2], xmVector.m128_f32[3])
	{}

	Color& operator=(const XMVector& xmVector);

	//-------------------------------------------------------------------------

	const float& operator[](const unsigned idx) const { return _data[idx]; }
	float& operator[](const unsigned idx) { return _data[idx]; }
	operator const float*() const { return _data.data(); }

	//-------------------------------------------------------------------------

	explicit operator XMVector() const;
	explicit operator const XMVector4&() const;
	explicit operator bool() const;

	//-------------------------------------------------------------------------

	static const Color Zero;

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

	//-------------------------------------------------------------------------

	std::string ToString() const;

	//-------------------------------------------------------------------------
	// logical operators
	//-------------------------------------------------------------------------

	bool operator==(const Color& other) const;
	bool operator!=(const Color& other) const;
	bool operator<(const Color& other) const;
	bool operator<=(const Color& other) const;
	bool operator>(const Color& other) const;
	bool operator>=(const Color& other) const;

	//-------------------------------------------------------------------------
	// arithmetic operators
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// add
	//-------------------------------------------------------------------------

	Color operator+(const Color& other) const;
	Color& operator+=(const Color& other);

	//-------------------------------------------------------------------------

	Color operator+(const float value) const;
	Color& operator+=(const float value);

	//-------------------------------------------------------------------------
	// negate
	//-------------------------------------------------------------------------

	Color operator-() const;

	//-------------------------------------------------------------------------
	// subtract
	//-------------------------------------------------------------------------

	Color operator-(const Color& other) const;
	Color& operator-=(const Color& other);

	//-------------------------------------------------------------------------

	Color operator-(const float value) const;
	Color& operator-=(const float value);

	//-------------------------------------------------------------------------
	// multiply
	//-------------------------------------------------------------------------

	Color operator*(const Color& other) const;
	Color& operator*=(const Color& other);

	//-------------------------------------------------------------------------

	Color operator*(const float value) const;
	Color& operator*=(const float value);

	//-------------------------------------------------------------------------
	// divide
	//-------------------------------------------------------------------------

	Color operator/(const Color& other) const;
	Color& operator/=(const Color& other);

	//-------------------------------------------------------------------------

	Color operator/(const float value) const;
	Color& operator/=(const float value);
};

//-------------------------------------------------------------------------
// Lerp
//-------------------------------------------------------------------------

Color Lerp(const Color& lhs, const Color& rhs, const float factor);
Color Lerp(const Color& lhs, const Color& rhs, const Vector4& factor);

} // namespace library::math
