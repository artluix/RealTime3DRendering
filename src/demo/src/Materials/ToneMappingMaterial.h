#pragma once
#include <library/Materials/PostProcessingMaterial.h>

class ToneMappingMaterial : public library::PostProcessingMaterial
{
	RTTI_CLASS(ToneMappingMaterial, library::PostProcessingMaterial)

public:
	explicit ToneMappingMaterial(library::Effect& effect);
	~ToneMappingMaterial();
};
