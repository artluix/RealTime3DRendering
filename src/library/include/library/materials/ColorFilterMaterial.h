#pragma once
#include "library/Materials/PostProcessingMaterial.h"

namespace library
{
class ColorFilterMaterial : public PostProcessingMaterial
{
	RTTI_CLASS(ColorFilterMaterial, PostProcessingMaterial)

public:
	explicit ColorFilterMaterial(std::shared_ptr<Effect> effect);
	~ColorFilterMaterial();

	EffectVariable& GetColorFilter() { return m_colorFilter; }

private:
	EffectVariable& m_colorFilter;
};
} // namespace library
