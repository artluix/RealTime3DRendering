#include "StdAfx.h"
#include "library/Math/Color.h"

#include <iomanip>
#include <sstream>
#include <random>

namespace library::math
{
Color Color::Random(const float a /* = 1.f */)
{
	static std::random_device s_device;
	static std::default_random_engine k_generator(s_device());
	static const std::uniform_real_distribution<float> k_distribution(0.f, 1.f);

	float r = k_distribution(k_generator);
	float g = k_distribution(k_generator);
	float b = k_distribution(k_generator);

	return Color(r, g, b, a);
}

//-------------------------------------------------------------------------

Color Clamp(const Color& c, const Color& min, const Color& max)
{
	return Color(Clamp(
		static_cast<const Vector4&>(c),
		static_cast<const Vector4&>(min),
		static_cast<const Vector4&>(max)
	));
}
Color Lerp(const Color& from, const Color& to, const float factor)
{
	return Color(Lerp(
		static_cast<const Vector4&>(from),
		static_cast<const Vector4&>(to),
		factor
	));
}
Color Lerp(const Color& from, const Color& to, const Vector4& factor)
{
	return Color(Lerp(
		static_cast<const Vector4&>(from),
		static_cast<const Vector4&>(to),
		factor
	));
}

} // namespace library::math
