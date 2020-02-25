#pragma once
#include "Materials/LightMaterial.h"

class SpotlightMaterial : public LightMaterial
{
	RTTI_CLASS(SpotlightMaterial, LightMaterial)

public:
	explicit SpotlightMaterial(library::Effect& effect, const std::string& defaultTechniqueName = "main11")
		: LightMaterial(effect, defaultTechniqueName)
	{}
};
