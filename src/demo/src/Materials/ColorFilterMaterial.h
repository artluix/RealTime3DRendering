#pragma once
#include <library/Materials/PostProcessingMaterial.h>

class ColorFilterMaterial : public library::PostProcessingMaterial
{
	RTTI_CLASS(ColorFilterMaterial, library::PostProcessingMaterial)

public:
	explicit ColorFilterMaterial(library::Effect& effect);
	~ColorFilterMaterial();

	library::EffectVariable& GetColorFilter() { return m_colorFilter; }

private:
	library::EffectVariable& m_colorFilter;
};
