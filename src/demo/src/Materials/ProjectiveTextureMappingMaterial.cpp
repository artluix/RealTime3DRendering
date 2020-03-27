#include "Materials/ProjectiveTextureMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

ProjectiveTextureMappingMaterial::ProjectiveTextureMappingMaterial(Effect& effect)
	: PhongLightingMaterial(effect, "project_texture")

	, m_projectedTexture(effect.GetVariable("ProjectedTexture"))
	, m_projectiveTextureMatrix(effect.GetVariable("ProjectiveTextureMatrix"))
	, m_depthMapTextureMatrix(effect.GetVariable("DepthMapTexture"))
	, m_depthBias(effect.GetVariable("DepthBias"))
{}

//-------------------------------------------------------------------------

void ProjectiveTextureMappingMaterial::InitializeInternal()
{
	PhongLightingMaterial::InitializeInternal();

	const auto& inputElementDescriptions = GetCurrentPass().GetInputElementDescriptions();

	CreateInputLayout(inputElementDescriptions, "project_texture_depth_map");
}
