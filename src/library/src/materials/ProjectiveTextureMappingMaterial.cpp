#include "StdAfx.h"
#include "library/Materials/ProjectiveTextureMappingMaterial.h"

#include "library/Effect/Effect.h"

namespace library
{
ProjectiveTextureMappingMaterial::ProjectiveTextureMappingMaterial(Effect& effect)
	: PointLightMaterial(effect, "project_texture")

	, m_projectedTexture(effect.GetVariable("ProjectedTexture"))
	, m_projectiveTextureMatrix(effect.GetVariable("projectiveTextureMatrix"))
	, m_depthMapTextureMatrix(effect.GetVariable("DepthMapTexture"))
	, m_depthBias(effect.GetVariable("depthBias"))
{}

//-------------------------------------------------------------------------

void ProjectiveTextureMappingMaterial::InitializeInternal()
{
	PointLightMaterial::InitializeInternal();

	CreateInputLayout("project_texture_depth_map");
}
} // namespace library
