#pragma once
#include "library/Materials/LightMaterial.h"

namespace library
{
class DirectionalLightMaterial : public LightMaterial
{
	RTTI_CLASS(DirectionalLightMaterial, Material)

public:
	explicit DirectionalLightMaterial(
		std::shared_ptr<Effect> effect, const std::string& defaultTechniqueName = "main11");
	~DirectionalLightMaterial();

	EffectVariable& GetLightDirection() { return m_lightDirection; }

private:
	EffectVariable& m_lightDirection;
};
} // namespace library
