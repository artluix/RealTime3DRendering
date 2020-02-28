#pragma once
#include <library/Components/SpotLightComponent.h>

__declspec(align(16))
struct DirectionalLightData
{
	using ComponentType = library::DirectionalLightComponent;

	library::math::Color color;
	library::math::Vector3 direction;

	DirectionalLightData() = default;
	explicit DirectionalLightData(const ComponentType& dirLight)
		: color(dirLight.GetColor())
		, direction(dirLight.GetDirection())
	{
	}

	DirectionalLightData& operator=(const ComponentType& dirLight)
	{
		color = dirLight.GetColor();
		direction = dirLight.GetDirection();
		return *this;
	}
};

//-------------------------------------------------------------------------

__declspec(align(16))
struct PointLightData
{
	using ComponentType = library::PointLightComponent;

	library::math::Color color;
	library::math::Vector3 position;
	float radius;

	PointLightData() = default;
	explicit PointLightData(const ComponentType& pointLight)
		: color(pointLight.GetColor())
		, position(pointLight.GetPosition())
		, radius(pointLight.GetRadius())
	{
	}

	PointLightData& operator=(const ComponentType& pointLight)
	{
		color = pointLight.GetColor();
		position = pointLight.GetPosition();
		radius = pointLight.GetRadius();
		return *this;
	}
};

//-------------------------------------------------------------------------

__declspec(align(16))
struct SpotlightData
{
	using ComponentType = library::SpotlightComponent;

	library::math::Color color;
	library::math::Vector3 direction;
	float radius = 0.f;
	library::math::Vector3 position;
	float innerAngle = 0.f, outerAngle = 0.f;

	SpotlightData() = default;
	explicit SpotlightData(const ComponentType& spotlight)
		: color(spotlight.GetColor())
		, direction(spotlight.GetDirection())
		, radius(spotlight.GetRadius())
		, position(spotlight.GetPosition())
		, innerAngle(spotlight.GetInnerAngle())
		, outerAngle(spotlight.GetOuterAngle())
	{
	}

	SpotlightData& operator=(const ComponentType& spotlight)
	{
		color = spotlight.GetColor();
		direction = spotlight.GetDirection();
		radius = spotlight.GetRadius();
		position = spotlight.GetPosition();
		innerAngle = spotlight.GetInnerAngle();
		outerAngle = spotlight.GetOuterAngle();
		return *this;
	}
};

//-------------------------------------------------------------------------

struct LightsData
{
	std::vector<DirectionalLightData> dirLights;
	unsigned dirLightsCount;

	std::vector<PointLightData> pointLights;
	unsigned pointLightsCount;

	std::vector<DirectionalLightData> spotlights;
	unsigned spotlightsCount;
};
