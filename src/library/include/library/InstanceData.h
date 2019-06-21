#pragma once
#include "library/Math/Matrix.h"
#include "library/Math/Color.h"

namespace library
{
struct InstanceData
{
	InstanceData(const math::Matrix4& world, const math::Color& specularColor, const float specularPower)
		: world(world)
		, specularColor(specularColor)
		, specularPower(specularPower)
	{}

	math::Matrix4 world;
	math::Color specularColor;
	float specularPower;
};
} // namespace library
