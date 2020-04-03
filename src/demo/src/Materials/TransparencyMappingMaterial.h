#pragma once
#include "Materials/FogMaterial.h"

class TransparencyMappingMaterial : public FogMaterial
{
	RTTI_CLASS(TransparencyMappingMaterial, FogMaterial)

public:
	explicit TransparencyMappingMaterial(library::Effect& effect);
	~TransparencyMappingMaterial();

	library::EffectVariable& GetTransparencyMap() { return m_transparencyMap; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_transparencyMap;
};
