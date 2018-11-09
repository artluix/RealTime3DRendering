#include "StdAfx.h"
#include "library/Color.h"

#include <random>

namespace library
{
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
