#include "Materials/ProjectiveTextureMappingMaterial.h"

#include <library/Effect/Effect.h>

using namespace library;

ProjectiveTextureMappingMaterial::ProjectiveTextureMappingMaterial(Effect& effect)
	: PhongLightingMaterial(effect, "project_texture")

	, m_projectedTexture(effect.GetVariable("ProjectedTexture"))
	, m_projectiveTextureMatrix(effect.GetVariable("ProjectiveTextureMatrix"))
	, m_depthMapTextureMatrix(effect.GetVariable("DepthMapTexture"))
	, m_depthBias(effect.GetVariable("DepthBias"))

	, m_cameraPosition(effect.GetVariable("CameraPosition"))
	, m_wvp(effect.GetVariable("WVP"))
	, m_world(effect.GetVariable("World"))
	, m_colorTexture(effect.GetVariable("ColorTexture"))
{}

//-------------------------------------------------------------------------

void ProjectiveTextureMappingMaterial::InitializeInternal()
{
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), GetDefaultTechniqueName());
	CreateInputLayout(MakeArrayBuffer(Vertex::ElementDescriptions), "project_texture_depth_map");
}
