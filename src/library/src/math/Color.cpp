#include "StdAfx.h"
#include "library/Math/Color.h"

#include <iomanip>
#include <sstream>
#include <random>

namespace library::math
{

Color& Color::operator=(const XMVector& xmVector)
{
	DirectX::XMStoreFloat2(reinterpret_cast<XMVector2*>(this), xmVector);
	return *this;
}

//-------------------------------------------------------------------------

Color::operator XMVector() const
{
	return DirectX::XMLoadFloat4(reinterpret_cast<const XMVector4*>(this));
}

Color::operator const XMVector4&() const
{
	return reinterpret_cast<const XMVector4&>(*this);
}

Color::operator bool() const
{
	static constexpr Color zero;
	return *this != zero;
}

//-------------------------------------------------------------------------

constexpr Color Color::Zero;

constexpr Color Color::White{1.f, 1.f, 1.f, 1.f};
constexpr Color Color::Black{0.f, 0.f, 0.f, 1.f};
constexpr Color Color::Red{1.f, 0.f, 0.f, 1.f};
constexpr Color Color::Green{0.f, 1.f, 0.f, 1.f};
constexpr Color Color::Blue{0.f, 0.f, 1.f, 1.f};
constexpr Color Color::Cyan{0.f, 1.f, 1.f, 1.f};
constexpr Color Color::Magenta{1.f, 0.f, 1.f, 1.f};
constexpr Color Color::Yellow{1.f, 1.f, 0.f, 1.f};
constexpr Color Color::CornFlower{0.392156899f, 0.584313750f, 0.929411829f, 1.000000000f};

//-------------------------------------------------------------------------

Color Color::Random()
{
	static std::random_device s_device;
	static std::default_random_engine k_generator(s_device());
	static const std::uniform_real_distribution<float> k_distribution(0.f, 1.f);

	float r = k_distribution(k_generator);
	float g = k_distribution(k_generator);
	float b = k_distribution(k_generator);

	return Color(r, g, b, 1.f);
}

//-------------------------------------------------------------------------

std::string Color::ToString() const
{
	std::ostringstream oss;
	oss << std::setprecision(4) << '(' << r << ", " << g << ", " << b << ", " << a << ')';
	return oss.str();
}

//-------------------------------------------------------------------------
// logical operators
//-------------------------------------------------------------------------

bool Color::operator==(const Color& other) const
{
	return DirectX::XMVector4Equal(XMVector(*this), XMVector(other));
}

bool Color::operator!=(const Color& other) const
{
	return DirectX::XMVector4NotEqual(XMVector(*this), XMVector(other));
}

bool Color::operator<(const Color& other) const
{
	return DirectX::XMVector4Less(XMVector(*this), XMVector(other));
}

bool Color::operator<=(const Color& other) const
{
	return DirectX::XMVector4LessOrEqual(XMVector(*this), XMVector(other));
}

bool Color::operator>(const Color& other) const
{
	return DirectX::XMVector4Greater(XMVector(*this), XMVector(other));
}

bool Color::operator>=(const Color& other) const
{
	return DirectX::XMVector4GreaterOrEqual(XMVector(*this), XMVector(other));
}

//-------------------------------------------------------------------------
// arithmetic operators
//-------------------------------------------------------------------------

//-------------------------------------------------------------------------
// add
//-------------------------------------------------------------------------

Color Color::operator+(const Color& other) const
{
	return Color(DirectX::XMVectorAdd(XMVector(*this), XMVector(other)));
}

Color& Color::operator+=(const Color& other)
{
	return *this = DirectX::XMVectorAdd(XMVector(*this), XMVector(other));
}

//-------------------------------------------------------------------------

Color Color::operator+(const float value) const
{
	return *this + Color(value);
}

Color& Color::operator+=(const float value)
{
	return *this += Color(value);
}

//-------------------------------------------------------------------------
// negate
//-------------------------------------------------------------------------

Color Color::operator-() const
{
	return Color(DirectX::XMVectorNegate(XMVector(*this)));
}

//-------------------------------------------------------------------------
// subtract
//-------------------------------------------------------------------------

Color Color::operator-(const Color& other) const
{
	return Color(DirectX::XMVectorSubtract(XMVector(*this), XMVector(other)));
}

Color& Color::operator-=(const Color& other)
{
	return *this = DirectX::XMVectorSubtract(XMVector(*this), XMVector(other));
}

//-------------------------------------------------------------------------

Color Color::operator-(const float value) const
{
	return *this - Color(value);
}

Color& Color::operator-=(const float value)
{
	return *this -= Color(value);
}

//-------------------------------------------------------------------------
// multiply
//-------------------------------------------------------------------------

Color Color::operator*(const Color& other) const
{
	return Color(DirectX::XMVectorMultiply(XMVector(*this), XMVector(other)));
}

Color& Color::operator*=(const Color& other)
{
	return *this = DirectX::XMVectorMultiply(XMVector(*this), XMVector(other));
}

//-------------------------------------------------------------------------

Color Color::operator*(const float value) const
{
	return *this * Color(value);
}

Color& Color::operator*=(const float value)
{
	return *this *= Color(value);
}

//-------------------------------------------------------------------------
// divide
//-------------------------------------------------------------------------

Color Color::operator/(const Color& other) const
{
	return Color(DirectX::XMVectorDivide(XMVector(*this), XMVector(other)));
}

Color& Color::operator/=(const Color& other)
{
	return *this = DirectX::XMVectorDivide(XMVector(*this), XMVector(other));
}

//-------------------------------------------------------------------------

Color Color::operator/(const float value) const
{
	return *this / Color(value);
}

Color& Color::operator/=(const float value)
{
	return *this /= Color(value);
}

//-------------------------------------------------------------------------
// Lerp
//-------------------------------------------------------------------------

Color Lerp(const Color& lhs, const Color& rhs, const float factor)
{
	return Color(DirectX::XMVectorLerp(XMVector(lhs), XMVector(rhs), factor));
}

Color Lerp(const Color& lhs, const Color& rhs, const Vector4& factor)
{
	return Color(DirectX::XMVectorLerpV(XMVector(lhs), XMVector(rhs), XMVector(factor)));
}
} // namespace library::math
