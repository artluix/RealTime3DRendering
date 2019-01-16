#include "StdAfx.h"
#include "library/Color.h"

#include <random>

namespace library
{
	constexpr Color Color::White = Color(1.f, 1.f, 1.f, 1.f);
	constexpr Color Color::Black = Color(0.f, 0.f, 0.f, 1.f);
	constexpr Color Color::Red = Color(1.f, 0.f, 0.f, 1.f);
	constexpr Color Color::Green = Color(0.f, 1.f, 0.f, 1.f);
	constexpr Color Color::Blue = Color(0.f, 0.f, 1.f, 1.f);
	constexpr Color Color::Cyan = Color(0.f, 1.f, 1.f, 1.f);
	constexpr Color Color::Magenta = Color(1.f, 0.f, 1.f, 1.f);
	constexpr Color Color::Yellow = Color(1.f, 1.f, 0.f, 1.f);
	constexpr Color Color::CornFlower = Color(0.392156899f, 0.584313750f, 0.929411829f, 1.000000000f);

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
} // namespace library
