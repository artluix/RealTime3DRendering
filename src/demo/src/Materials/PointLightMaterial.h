#pragma once
#include "Materials/LightMaterial.h"

class PointLightMaterial : public LightMaterial
{
	RTTI_CLASS(PointLightMaterial, LightMaterial)

public:
	explicit PointLightMaterial(library::Effect& effect, const std::string& defaultTechniqueName = "main11")
		: LightMaterial(effect, defaultTechniqueName)
	{}
};
