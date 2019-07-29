#include "Materials/ProjectiveTextureMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

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

	const auto& inputElementDescriptions = GetCurrentPass().GetInputElementDescriptions();

	CreateInputLayout(inputElementDescriptions, "project_texture_depth_map");
}
