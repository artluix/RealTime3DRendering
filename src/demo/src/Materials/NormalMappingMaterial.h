#pragma once
#include "Materials/PhongLightingMaterial.h"

class NormalMappingMaterial : public PhongLightingMaterial
{
	RTTI_CLASS(NormalMappingMaterial, PhongLightingMaterial)

public:
	using Vertex = library::VertexPositionTextureNormalTangent;

	explicit NormalMappingMaterial(library::Effect& effect);
	~NormalMappingMaterial();

	library::EffectVariable& GetNormalMap() { return m_normalMap; }

	library::EffectVariable& GetCameraPosition() { return m_cameraPosition; }
	library::EffectVariable& GetWVP() { return m_wvp; }
	library::EffectVariable& GetWorld() { return m_world; }
	library::EffectVariable& GetColorTexture() { return m_colorTexture; }

protected:
	void InitializeInternal() override;

private:
	library::EffectVariable& m_normalMap;

	library::EffectVariable& m_cameraPosition;
	library::EffectVariable& m_wvp;
	library::EffectVariable& m_world;
	library::EffectVariable& m_colorTexture;
};
