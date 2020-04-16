#include "StdAfx.h"
#include "library/Materials/EquirectangularToCubeMapMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
EquirectangularToCubeMapMaterial::EquirectangularToCubeMapMaterial(Effect& effect)
	: Material(effect, "main11")

	, m_viewProjection(effect.GetVariable("ViewProjection"))
	, m_texture(effect.GetVariable("EquirectangularMap"))
{
}

void EquirectangularToCubeMapMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
}

} // namespace library
