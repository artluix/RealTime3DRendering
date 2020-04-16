#pragma once
#include "library/Materials/Material.h"

namespace library
{
class EquirectangularToCubeMapMaterial : public Material
{
	RTTI_CLASS(EquirectangularToCubeMapMaterial, Material)

public:
	using Vertex = VertexPosition;

	EquirectangularToCubeMapMaterial(Effect& effect);

	EffectVariable& GetViewProjection() { return m_viewProjection; }
	EffectVariable& GetTexture() { return m_texture; }

protected:
	void InitializeInternal() override;

private:
	EffectVariable& m_viewProjection;
	EffectVariable& m_texture;
};
} // namespace library
